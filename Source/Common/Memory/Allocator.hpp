//https://github.com/awwdev

#pragma once
#include "Common/Types.hpp"
#include "Common/Container/Bitset.hpp"
#include "Debug/Logger.hpp"
#include "Common/Container/String.hpp"

#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "psapi.h"

//? note: do not claim a block per array element, instead claim one block for the whole array

namespace rpg::com::mem {

constexpr auto DO_LOG_BLOCKS = false;

struct BlockArray
{
    u32 size;
    u32 count;
};

//?-------------------------------------
//?DEFINE BLOCKS HERE (keep sorted for size!)
constexpr BlockArray BLOCK_ARRAYS [] {
    { .size =        1'000, .count = 5  },
    { .size =       10'000, .count = 5  },
    { .size =      100'000, .count = 5  },
    { .size =    1'000'000, .count = 5  },
    { .size =   10'000'000, .count = 5  },
    { .size =  100'000'000, .count = 5  },
};
//?-------------------------------------

constexpr auto BLOCK_ARRAY_COUNT = ArrayCount(BLOCK_ARRAYS);
constexpr auto BLOCK_COUNT_TOTAL = []() constexpr {
    std::size_t count = 0;
    FOR_CARRAY(BLOCK_ARRAYS, i)
        count += BLOCK_ARRAYS[i].count;
    return count;
}();

constexpr auto ALLOCATION_SIZE = []() constexpr {
    std::size_t size = 0;
    FOR_CARRAY(BLOCK_ARRAYS, i)
        size += BLOCK_ARRAYS[i].size * BLOCK_ARRAYS[i].count;
    return size;
}();

namespace priv 
{
    //all blocks are successive 
    inline com::Bitset<BLOCK_COUNT_TOTAL> blocksUsed;
    inline u8* blockArrayPtrs [BLOCK_ARRAY_COUNT]; //u8 for ptr arithmetics 

    struct BlockInfo 
    {
        com::String<300> fnName;
        std::uintptr_t   address;
    };
    inline BlockInfo blockInfos [BLOCK_COUNT_TOTAL];

    template<idx_t ARRAY_INDEX>
    u8* BlockAddress(const idx_t arrBlockIdx) 
    {
        constexpr auto blockSize = BLOCK_ARRAYS[ARRAY_INDEX].size;
        return priv::blockArrayPtrs[ARRAY_INDEX] + (arrBlockIdx * blockSize);
    }

    inline void FreeBlock(const idx_t blockId)
    {
        priv::blocksUsed.Flip(blockId);
        priv::blockInfos[blockId].fnName.Clear();
        priv::blockInfos[blockId].address = 0;
        //dtor call of object is done in BlockPtr

        if constexpr(DO_LOG_BLOCKS)
            dbg::LogColor(dbg::ConsoleColors::Red, "FreeBlock", blockId);
    }
}

inline void GlobalAllocate()
{
    priv::blockArrayPtrs[0] = (u8*)VirtualAlloc(NULL, ALLOCATION_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    dbg::LogInfo("Block array:", 0, BLOCK_ARRAYS[0].size * BLOCK_ARRAYS[0].count, (void*)priv::blockArrayPtrs[0]);

    for(idx_t i = 1; i < BLOCK_ARRAY_COUNT; ++i) {
        priv::blockArrayPtrs[i] = priv::blockArrayPtrs[i-1] + BLOCK_ARRAYS[i-1].size * BLOCK_ARRAYS[i-1].count; 
        dbg::LogInfo("Block array:", i, BLOCK_ARRAYS[i].size * BLOCK_ARRAYS[i].count, (void*)priv::blockArrayPtrs[i]);
    }
}

inline void GlobalDeallocate()
{
    VirtualFree(priv::blockArrayPtrs[0], 0, MEM_RELEASE);
}

template<class T>
struct BlockPtr
{
    using DATA_T = T;

    T* ptr = nullptr;
    u32 blockId = 0;

    T* operator->() { return  ptr; }
    T& operator* () { return *ptr; }

    T&       Get()       { return *ptr; }
    const T& Get() const { return *ptr; }

    //array access
    auto&       operator[](const std::size_t i)        { return (*ptr)[i]; };
    const auto& operator[](const std::size_t i) const  { return (*ptr)[i]; };

    BlockPtr(T* const pPtr, const u32 pBlockId)
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
        if constexpr(!std::is_trivial_v<T>)
        {
            if constexpr(std::is_array_v<T>)
            {
                using Element = std::remove_all_extents_t<T>;
                constexpr auto COUNT = sizeof(T)/sizeof(Element);
                for(idx_t i = 0; i < COUNT; ++i)
                    ((Element*)ptr + i)->~Element();
            }
            else 
            {
                ptr->~T(); 
            }
        }
        priv::FreeBlock(blockId);
    }
};

template<class T, class... CtorArgs>
auto ClaimBlock(CtorArgs&&... args)
{
    struct FittingBlockSize { u32 arrayIdx; u32 blockId; }; //blockId == bitIdx

    constexpr auto FITTING_BLOCK_ARRAY = []() constexpr -> FittingBlockSize {
        u32 blockId = 0;
        for(u32 i = 0; i < BLOCK_ARRAY_COUNT; ++i) {
            if (BLOCK_ARRAYS[i].size >= (sizeof(T) + alignof(T))) //max
                return FittingBlockSize { i, blockId };
            blockId += BLOCK_ARRAYS[i].count;
        }
        //!no appropriate block size found (compile time error)
    }();

    const auto freeBlockId = priv::blocksUsed.FindFirstFreeBit(FITTING_BLOCK_ARRAY.blockId);
    dbg::Assert(freeBlockId < FITTING_BLOCK_ARRAY.blockId + BLOCK_ARRAYS[FITTING_BLOCK_ARRAY.arrayIdx].count, "not enough free blocks");
    priv::blocksUsed.Flip(freeBlockId);

    u8* blockAddress = priv::BlockAddress<FITTING_BLOCK_ARRAY.arrayIdx>(freeBlockId - FITTING_BLOCK_ARRAY.blockId);
    u8* aligned      = (u8*) (((std::uintptr_t)blockAddress + (alignof(T) - 1)) & ~(alignof(T) - 1));
    
    T* const obj = [&]{
        if constexpr (std::is_array_v<T>) {
            struct Arr { T data; };
            return &(new (aligned) Arr { std::forward<CtorArgs>(args) ... })->data;
        }   
        else return new (aligned) T  { std::forward<CtorArgs>(args) ... };
    }();

    //? debug info
    priv::blockInfos[freeBlockId].fnName  = __PRETTY_FUNCTION__;
    priv::blockInfos[freeBlockId].address = (std::uintptr_t) aligned;

    if constexpr(DO_LOG_BLOCKS)
        dbg::LogColor(dbg::ConsoleColors::Red, "ClaimBlock", 
            "arr idx ", FITTING_BLOCK_ARRAY.arrayIdx, 
            "block id ", freeBlockId - FITTING_BLOCK_ARRAY.blockId, 
            "freeBlockId ", freeBlockId,
            __PRETTY_FUNCTION__);
            
    return BlockPtr<T> { obj, freeBlockId };
}

inline idx_t GetBlockArrayIdxFromBlockId(const idx_t blockId)
{
    idx_t blockCount = 0;
    FOR_CARRAY(BLOCK_ARRAYS, i){
        blockCount += BLOCK_ARRAYS[i].count;
        if (blockId < blockCount)
            return i;
    }
    dbg::LogError("cannot get block array idx from block id");
    return 0;
}

}//ns