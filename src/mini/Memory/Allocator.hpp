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
        {  128, 10 },  //used for strings
        { 1000, 10 },
        { 3000, 10 },  //vk renderer
        { 8000, 10 },  //32*32*4 bmp = 4096
        { 10001, 10 }  //shader file buffer
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

    inline u8* allocPtrs [ALLOC_COUNT];         //index based
    inline box::Bitset<BLOCK_COUNT> blocksUsed; //all blocks


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


    template<class T>
    struct BlockPtr
    {
        T* ptr = nullptr;
        std::size_t blockId = 0;

        T* operator->() { return ptr; }
        T& operator* () { return *ptr;}

        T&       Get()       { return *ptr; }
        const T& Get() const { return *ptr; }

        //array access
        auto&       operator[](const std::size_t i)        { return (*ptr)[i]; };
        const auto& operator[](const std::size_t i) const  { return (*ptr)[i]; };

        BlockPtr() = default;
        BlockPtr(T* const pPtr, const std::size_t pBlockId)
            : ptr     { pPtr }
            , blockId { pBlockId } 
        {;}

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
            blocksUsed.Flip(blockId); //since its global we dont need to store a ref to the bitset
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
                if (ALLOC_INFOS[i].blockSize >= (sizeof(T) + alignof(T))) //assumes sorted
                    return { i, allocBitBegin };
                allocBitBegin += ALLOC_INFOS[i].blockCount;
            }
            //will not compile when hitting this point
        }();

        const auto freeBlock = blocksUsed.FindFirstFreeBit(FIT.allocBitBegin); //start search at the alloc
        blocksUsed.Flip(freeBlock); //mark used

        constexpr auto blockSize = ALLOC_INFOS[FIT.allocIdx].blockSize;
        auto* ptr     = allocPtrs[FIT.allocIdx] + ((freeBlock - FIT.allocBitBegin) * blockSize);
        auto* aligned = (u8*) (((std::uintptr_t)ptr + (alignof(T) - 1)) & ~(alignof(T) - 1)); //next aligned address

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

//TODO: #6 mem::PrintBlocks()
