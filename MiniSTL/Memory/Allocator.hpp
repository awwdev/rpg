#pragma once
#include "MiniSTL/Types.hpp"
#include "MiniSTL/Container/Bitset.hpp"
#include "MiniSTL/Debug/Logger.hpp"

#define WIN32_LEAN_AND_MEAN
#include "windows.h"

namespace mini::mem
{
    //global purpose pool allocator (pure functions)
    //sizes in bytes
    //define desired blocks and sizes at compile time


    template<std::size_t BLOCK_SIZE_T, std::size_t BLOCK_COUNT_T>
    struct Allocation
    {
        static constexpr auto BLOCK_SIZE  = BLOCK_SIZE_T;
        static constexpr auto BLOCK_COUNT = BLOCK_COUNT_T;
        static constexpr auto ALLOC_SIZE  = BLOCK_SIZE_T * BLOCK_COUNT_T;

        container::Bitset<BLOCK_COUNT_T> used { 0 };
        void* base = nullptr;
    };

    
#define BLOCK_POOL(bytes, count) inline Allocation<bytes, count> blocks_##bytes;
#define BLOCK_POOL_FROM_SIZE(obj, bytes) if constexpr (sizeof(obj) <= blocks_##bytes.BLOCK_SIZE) return blocks_##bytes;
#define BLOCK_POOL_FROM_OWNER(blk, bytes) if constexpr (std::is_same_v<blk::BLOCK_TYPE, std::decay_t<decltype(blocks_##bytes)>>) return blocks_##bytes;
#define ALLOC_BLOCK_POOL(bytes) blocks_##bytes.base = HeapAlloc(heap, 0, blocks_##bytes.ALLOC_SIZE);
    
    ///DEFINE BLOCK POOLS ----------------------------------------------
    BLOCK_POOL(8, 1000)
    BLOCK_POOL(512, 1000)
    BLOCK_POOL(20000, 1000)
    
    template<class T> constexpr auto& GetBlockPoolFromObjectSize() {
             BLOCK_POOL_FROM_SIZE(T, 8)
        else BLOCK_POOL_FROM_SIZE(T, 512)
        else BLOCK_POOL_FROM_SIZE(T, 20000)
    };
    template<class T> constexpr auto& GetBlockPoolFromOwner() {
             BLOCK_POOL_FROM_OWNER(T, 8)
        else BLOCK_POOL_FROM_OWNER(T, 512)
        else BLOCK_POOL_FROM_OWNER(T, 20000)
    };

    inline void Allocate()
    {
        auto heap = GetProcessHeap();
        ALLOC_BLOCK_POOL(8)
        ALLOC_BLOCK_POOL(512)
        ALLOC_BLOCK_POOL(20000)
    }
    ///-----------------------------------------------------------------

#undef BLOCK_POOL
#undef BLOCK_POOL_FROM_SIZE
#undef BLOCK_POOL_FROM_TYPE
#undef ALLOC_BLOCK_POOL


    template<class T>
    void Free(const T& owner)
    {
        constexpr auto& blocks = GetBlockPoolFromOwner<T>();

        const auto blockDist = (u8*)owner.ptr - (u8*)blocks.base; //in bytes
        const auto blockNum  = blockDist / blocks.BLOCK_SIZE;

        blocks.used.Flip(blockNum);
    }


    template<class T, class B>
    struct MemOwner
    {
        using BLOCK_TYPE = B;

        T* ptr;
        T* operator->() { return ptr; }

        MemOwner() = delete;
        MemOwner(T* pPtr) : ptr { pPtr } {;}

        MemOwner(const MemOwner&)            = delete;
        MemOwner& operator=(const MemOwner&) = delete;

        MemOwner(MemOwner&& other) : ptr { other.ptr }  { other.ptr = nullptr; }
        MemOwner& operator=(MemOwner&& other)           { ptr = other.ptr; other.ptr = nullptr; return *this; }

        ~MemOwner() { if (ptr != nullptr) { ptr->~T(); Free(*this); } }
    };

    template<class T>
    using BlocksType = std::decay_t<decltype(GetBlockPoolFromObjectSize<T>())>;

    template<class T>
    using MemOwnerX = MemOwner<T, BlocksType<T>>;

    
    template<class T, class... Args>
    auto Claim(Args&&... args)
    {
        constexpr auto& blocks  = GetBlockPoolFromObjectSize<T>();
        const auto freeBitIndex = blocks.used.FindFirstFreeBit(); // ==block

        if (freeBitIndex == u32max) 
            return MemOwnerX<T> { nullptr }; //error

        blocks.used.Flip(freeBitIndex);

        auto* const blockPtr = (u8*) blocks.base + (freeBitIndex * blocks.BLOCK_SIZE);
        auto* const objPtr   = new(blockPtr) T(std::forward<Args>(args)...);

        return MemOwnerX<T> { objPtr };
    }

}//ns