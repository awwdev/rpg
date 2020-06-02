//https://github.com/awwdev

/*
## mini::mem

- global functions, simple usage
- compile time (capacity based system)
    - user defines blocks at compile time in one place (see array allocs[])
    - when claiming memory, the appropriate block size will be figured out at compile time
- BlockPtr
    - is returned when claiming memory
    - holds data about the used block and will "free" the block when destroyed (RAII)
- free / used blocks are represented by one bitset
- printable
*/

#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Box/Bitset.hpp"
#include "mini/Box/Map.hpp"
#include "mini/Box/String.hpp"
#include "mini/Debug/Logger.hpp"

#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "psapi.h"
#undef max


namespace mini::mem
{
    struct AllocInfo
    {
        std::size_t blockSize;
        std::size_t blockCount;
    };

    //!--------------------------------------
    //!define blocks at compile time (size and count) 
    //!KEEP IT SORTED BY SIZE
    constexpr AllocInfo ALLOC_INFOS[] = {
        { 100, 1    },     //scene stack
        { 5'000,  1 },     //vk renderer
        { 12'000, 3 },     //shader char buffer + 32*32*4 tex array (containing 2 textures)
        { 10'000'000, 2 }  //1024*1024*4 tex array
    };
    //!--------------------------------------


    constexpr auto ALLOC_COUNT = ARRAY_COUNT(ALLOC_INFOS);
    constexpr auto BLOCK_COUNT = []() constexpr {
        std::size_t count = 0;
        for(auto i = 0; i < ALLOC_COUNT; ++i) {
            count += ALLOC_INFOS[i].blockCount;
        }
        return count;
    }();
    constexpr auto ALLOC_SIZE = []() constexpr { 
        std::size_t size = 0;
        for(auto i = 0; i < ALLOC_COUNT; ++i) {
            size += ALLOC_INFOS[i].blockSize * ALLOC_INFOS[i].blockCount;
        }
        return size;
    }();

    constexpr inline u32 GetAllocIdxFromBlockId(const u32 blockId)
    {
        u32 currBlockCount = 0;
        for(u32 i = 0; i < ALLOC_COUNT; ++i)
        {
            currBlockCount += ALLOC_INFOS[i].blockCount;
            if (blockId < currBlockCount)
                return i;
        }
        ERR("current block count wrong");
        return 0;
    }


    //?GLOBAL ----------------------------------------------
    inline u8* allocPtrs [ALLOC_COUNT];         //index based
    inline box::Bitset<BLOCK_COUNT> blocksUsed; //all blocks
    inline box::Map<box::String<200>, BLOCK_COUNT> blockTypes;
    //?-----------------------------------------------------


    inline void GlobalAllocate()
    {
        auto heapPtr = HeapAlloc(GetProcessHeap(), 0, ALLOC_SIZE);

        allocPtrs[0] = (u8*)heapPtr;
        LOG("allocation:", ALLOC_INFOS[0].blockSize * ALLOC_INFOS[0].blockCount, (void*)allocPtrs[0]);

        for(auto i = 1; i < ALLOC_COUNT; ++i) {
            allocPtrs[i] = (u8*)heapPtr + ALLOC_INFOS[i-1].blockSize * ALLOC_INFOS[i-1].blockCount; 
            LOG("allocation:", ALLOC_INFOS[i].blockSize * ALLOC_INFOS[i].blockCount, (void*)allocPtrs[i]);
        }
    }


    inline void GlobalDeallocate()
    {
        HeapFree(GetProcessHeap(), 0, allocPtrs[0]);
    }

    enum class AutoClaim { Yes, No };

    inline void FreeBlock(const u32 blockId)
    {
        //LOG("BLOCK FREE: ", typeid(T).name()); //lookup emplaced names
        blocksUsed.Flip(blockId); //since its global we dont need to store a ref to the bitset
        blockTypes.Remove(blockId);
    }


    template<class T>
    struct BlockPtr
    {
        using DATA_T = T;

        T* ptr = nullptr;
        std::size_t blockId = 0;

        T* operator->() { return ptr; }
        T& operator* () { return *ptr;}

        T&       Get()       { return *ptr; }
        const T& Get() const { return *ptr; }

        //array access
        auto&       operator[](const std::size_t i)        { return (*ptr)[i]; };
        const auto& operator[](const std::size_t i) const  { return (*ptr)[i]; };

        explicit BlockPtr(const AutoClaim doAutoClaim = AutoClaim::No)
        {
            if (doAutoClaim == AutoClaim::Yes)
                ClaimBlock(*this);
        };
        BlockPtr(T* const pPtr, const std::size_t pBlockId)
            : ptr     { pPtr }
            , blockId { pBlockId } 
        {}

        BlockPtr(const BlockPtr&)            = delete;
        BlockPtr& operator=(const BlockPtr&) = delete;

        BlockPtr(BlockPtr&& other) 
            : ptr     { other.ptr }
            , blockId { other.blockId } 
        { 
            other.ptr = nullptr;
        }

        BlockPtr& operator=(BlockPtr&& other)           
        { 
            ptr         = other.ptr; 
            blockId     = other.blockId;
            other.ptr   = nullptr; 
            return *this; 
        }

        ~BlockPtr()
        { 
            if (ptr == nullptr) return;
            ptr->~T();
            FreeBlock(blockId);
        }
    };


    template<class T, class... CtorArgs>
    auto ClaimBlock(CtorArgs&&... args)
    {
        struct FittingAlloc 
        { 
            std::size_t allocIdx; 
            std::size_t allocBitBegin;
        };

        //!WHEN YOU GET A COMPILE ERROR HERE, ITS PROBABLY BECAUSE THERE IS NO BLOCK SIZE
        constexpr auto FIT = []() constexpr -> FittingAlloc {
            std::size_t allocBitBegin = 0;
            for(std::size_t i=0; i<ALLOC_COUNT; ++i) 
            {
                if (ALLOC_INFOS[i].blockSize >= (sizeof(T) + alignof(T))) //!assumes sorted
                    return { i, allocBitBegin };
                allocBitBegin += ALLOC_INFOS[i].blockCount;
            }
            //will not compile when hitting this point
        }();

        const auto freeBlock = blocksUsed.FindFirstFreeBit(FIT.allocBitBegin); //start search at the alloc
        blocksUsed.Flip(freeBlock); //mark used

        blockTypes.Set(freeBlock, typeid(T).name());

        constexpr auto blockSize = ALLOC_INFOS[FIT.allocIdx].blockSize;
        auto* ptr     = allocPtrs[FIT.allocIdx] + ((freeBlock - FIT.allocBitBegin) * blockSize);
        auto* aligned = (u8*) (((std::uintptr_t)ptr + (alignof(T) - 1)) & ~(alignof(T) - 1)); //next aligned address
        
        LOG("BLOCK CLAIM: ", typeid(T).name(), blockSize, (void*)ptr);

        T* obj = nullptr;
        if constexpr(std::is_array_v<T>)
        {
            obj = new (aligned) T[sizeof(T)];
        }
        else 
        {
            obj = new (aligned) T { std::forward<CtorArgs>(args) ... };
        }

        return BlockPtr<T> { obj, freeBlock };
    }


    template<class T, class... CtorArgs>
    void ClaimBlock(BlockPtr<T>& blockPtr, CtorArgs&&... args)
    {
        blockPtr = ClaimBlock<T>(args...);
    }


}//ns

//TODO: RUNTIME VERSION OF CLAIMBLOCK ?