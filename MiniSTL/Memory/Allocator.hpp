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


    inline Allocation<4,   1'000'000> blocks_4;
    inline Allocation<8,   1'000'000> blocks_8;
    inline Allocation<16,  1'000'000> blocks_16;
    inline Allocation<32,  1'000'000> blocks_32;
    inline Allocation<64,  1'000'000> blocks_64;
    inline Allocation<128, 200> blocks_128;
    inline Allocation<256, 1'000'000> blocks_256;
    inline Allocation<512, 1'000'000> blocks_512;


    inline void Allocate()
    {
        auto heap = GetProcessHeap();

        blocks_4.base   = HeapAlloc(heap, 0, blocks_4.ALLOC_SIZE);
        blocks_8.base   = HeapAlloc(heap, 0, blocks_8.ALLOC_SIZE);
        blocks_16.base  = HeapAlloc(heap, 0, blocks_16.ALLOC_SIZE);
        blocks_32.base  = HeapAlloc(heap, 0, blocks_32.ALLOC_SIZE);
        blocks_64.base  = HeapAlloc(heap, 0, blocks_64.ALLOC_SIZE);
        blocks_128.base = HeapAlloc(heap, 0, blocks_128.ALLOC_SIZE);
        blocks_256.base = HeapAlloc(heap, 0, blocks_256.ALLOC_SIZE);
        blocks_512.base = HeapAlloc(heap, 0, blocks_512.ALLOC_SIZE);

        DLOG("Allocation addresses", blocks_4.base, blocks_8.base, blocks_16.base, blocks_32.base, blocks_64.base, blocks_128.base, blocks_256.base, blocks_512.base);
        DLOG("Allocation sizes", blocks_4.ALLOC_SIZE, blocks_8.ALLOC_SIZE, blocks_16.ALLOC_SIZE, blocks_32.ALLOC_SIZE, blocks_64.ALLOC_SIZE, blocks_128.ALLOC_SIZE, blocks_256.ALLOC_SIZE, blocks_512.ALLOC_SIZE);
    }


    template<class T>
    void Free(const T& owner)
    {
        constexpr auto& blocks = []() constexpr -> auto& {
            if      constexpr (std::is_same_v<T::BLOCK_TYPE, std::decay_t<decltype(blocks_4)>>)   return blocks_4;
            else if constexpr (std::is_same_v<T::BLOCK_TYPE, std::decay_t<decltype(blocks_8)>>)   return blocks_8;
            else if constexpr (std::is_same_v<T::BLOCK_TYPE, std::decay_t<decltype(blocks_16)>>)  return blocks_16;
            else if constexpr (std::is_same_v<T::BLOCK_TYPE, std::decay_t<decltype(blocks_32)>>)  return blocks_32;
            else if constexpr (std::is_same_v<T::BLOCK_TYPE, std::decay_t<decltype(blocks_64)>>)  return blocks_64;
            else if constexpr (std::is_same_v<T::BLOCK_TYPE, std::decay_t<decltype(blocks_128)>>) return blocks_128;
            else if constexpr (std::is_same_v<T::BLOCK_TYPE, std::decay_t<decltype(blocks_256)>>) return blocks_256;
            else if constexpr (std::is_same_v<T::BLOCK_TYPE, std::decay_t<decltype(blocks_512)>>) return blocks_512;
        }();

        const auto blockDist = (u8*)owner.ptr - (u8*)blocks.base; //in bytes
        const auto blockNum  = blockDist / blocks.BLOCK_SIZE;

        DLOG("Freeing", blocks.BLOCK_SIZE, blockNum)

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
    constexpr auto& GetBlocksFromType() {
        if      constexpr (sizeof(T) <= 4)   return blocks_4;
        else if constexpr (sizeof(T) <= 8)   return blocks_8;
        else if constexpr (sizeof(T) <= 16)  return blocks_16;
        else if constexpr (sizeof(T) <= 32)  return blocks_32;
        else if constexpr (sizeof(T) <= 64)  return blocks_64;
        else if constexpr (sizeof(T) <= 128) return blocks_128;
        else if constexpr (sizeof(T) <= 256) return blocks_256;
        else if constexpr (sizeof(T) <= 512) return blocks_512;
    };

    template<class T>
    using BlocksType = std::decay_t<decltype(GetBlocksFromType<T>())>;

    template<class T>
    using MemOwnerX = MemOwner<T, BlocksType<T>>;


    template<class T, class... Args>
    auto Claim(Args&&... args)
    {
        constexpr auto& blocks  = GetBlocksFromType<T>();
        const auto freeBitIndex = blocks.used.FindFirstFreeBit(); // ==block

        if (freeBitIndex == u32max) 
            return MemOwnerX<T> { nullptr }; //error

        blocks.used.Flip(freeBitIndex);

        auto* const blockPtr = (u8*) blocks.base + (freeBitIndex * blocks.BLOCK_SIZE);
        auto* const objPtr   = new(blockPtr) T(std::forward<Args>(args)...);

        DLOG("Claim", sizeof(T), alignof(T), blocks.BLOCK_SIZE, (void*)blockPtr);
        return MemOwnerX<T> { objPtr };
    }

}//ns