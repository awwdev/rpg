//https://github.com/awwdev

/*
## mini::mem

- global functions, simple usage
- compile time (capacity based system)
    - user defines blocks at compile time in one place (see array allocs[])
    - when claiming memory, the appropriate block size will be figured out at compile time
- MemPtr
    - is returned when claiming memory
    - holds data about the used block and will "free" the block when destroyed (RAII)
- free / used blocks are represented by one bitset
- printable
*/

#pragma once
#include "mini/Types.hpp"
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
    //define blocks at compile time (size and count) 
    //KEEP IT SORTED BY SIZE
    constexpr AllocInfo ALLOC_INFOS[] = {
        {  128, 10 }, //used for strings
        { 1000, 10 },
        { 2000, 10 } //vk renderer
    };
    //!--------------------------------------


    constexpr auto ALLOC_COUNT = sizeof(ALLOC_INFOS) / sizeof(ALLOC_INFOS[0]);
    constexpr auto BLOCK_COUNT = []() constexpr { //total block count
        std::size_t count = 0;
        for(auto i = 0; i < ALLOC_COUNT; ++i) {
            count += ALLOC_INFOS[i].blockCount;
        }
        return count;
    }();

    inline u8* allocPtrs [ALLOC_COUNT];         //index based
    inline box::Bitset<BLOCK_COUNT> blocksUsed; //all blocks


    inline void GlobalAllocate()
    {
        HANDLE heap = GetProcessHeap();
        for(auto i = 0; i < ALLOC_COUNT; ++i) {
            allocPtrs[i] = (u8*)HeapAlloc(heap, 0, ALLOC_INFOS[i].blockSize * ALLOC_INFOS[i].blockCount); 
            LOG("Allocated:", ALLOC_INFOS[i].blockSize * ALLOC_INFOS[i].blockCount, (void*)allocPtrs[i]);
        }
    }


    inline void GlobalFree()
    {
        //todo
        //HeapFree 
    }


    template<class T>
    struct MemPtr
    {
        T* ptr;
        std::size_t blockId;

        T* operator->() { return ptr; }
        T& operator* () { return *ptr;}

        T&       Get()       { return *ptr; }
        const T& Get() const { return *ptr; }


        MemPtr() = delete;
        MemPtr(T* const pPtr, const std::size_t pBlockId)
            : ptr     { pPtr }
            , blockId { pBlockId } 
        {;}

        MemPtr(const MemPtr&)            = delete;
        MemPtr(MemPtr&&)                 = delete;
        MemPtr& operator=(const MemPtr&) = delete; //probably bad design if needed?
        MemPtr& operator=(MemPtr&&)      = delete;

        //TODO: #7 MemPtr move needed?
        //MemPtr(MemPtr&& other) 
        //    : ptr     { other.ptr }
        //    , blockId { other.blockId } 
        //{ 
        //    other.ptr = nullptr;
        //}

        //MemPtr& operator=(MemPtr&& other)           
        //{ 
        //    ptr         = other.ptr; 
        //    blockId     = other.blockId;
        //    other.ptr   = nullptr; 
        //    return *this; 
        //}

        ~MemPtr()
        { 
            //if (ptr != nullptr) //if we make memptr non movable we could just drop the if
            //{ 
                ptr->~T();
                blocksUsed.Flip(blockId); //since its global we dont need to store a ref to the bitset
            //} 
        }
    };


    //having oop ctor really help here instead of plain c functions
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
        auto* obj     = new (aligned) T (std::forward<CtorArgs>(args) ...);

        return MemPtr<T> { obj, freeBlock };
    }

}//ns

//TODO: #6 mem::PrintBlocks()