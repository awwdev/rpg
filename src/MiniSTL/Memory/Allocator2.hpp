#pragma once
#include "MiniSTL/Types.hpp"
#include "MiniSTL/Box/Bitset.hpp"
#include "MiniSTL/Box/Array.hpp"
#include "MiniSTL/Debug/Logger.hpp"

#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "psapi.h"
#undef max


namespace mini::mem2
{
    struct AllocInfo
    {
        u32 blockSize;
        u32 blockCount;
    };

    //!KEEP THIS SORTED BY SIZE
    constexpr AllocInfo allocs[] = {
        { 500, 100 },
        { 10000, 1 },
    };


    constexpr auto ALLOCS_COUNT = sizeof(allocs) / sizeof(allocs[0]);
    constexpr auto BIT_COUNT    = []() constexpr {
        std::size_t bitCount = 0;
        for(auto i=0; i<ALLOCS_COUNT; ++i) {
            bitCount += allocs[i].blockCount;
        }
        return bitCount;
    }();

    inline u8* allocPtrs [ALLOCS_COUNT]; //index based system
    inline mini::box::Bitset<BIT_COUNT> blocksAsBits; //all blocks together


    inline void Allocate()
    {
        auto heap = GetProcessHeap();
        for(auto i=0; i<ALLOCS_COUNT; ++i) {
            allocPtrs[i] = (u8*)HeapAlloc(heap, 0, allocs[i].blockSize); //alignment?
        }
    }


    template<class T>
    struct MemOwner
    {
        T* ptr;
        std::size_t blockBitIdx;

        T* operator->() { return ptr; }
        T& operator* () { return *ptr;}

        MemOwner() = delete;
        MemOwner(T* const pPtr, const std::size_t pBlockBitIdx) : ptr { pPtr }, blockBitIdx { pBlockBitIdx } {;}

        MemOwner(const MemOwner&)            = delete;
        MemOwner& operator=(const MemOwner&) = delete;

        MemOwner(MemOwner&& other) : ptr { other.ptr }  { other.ptr = nullptr; }
        MemOwner& operator=(MemOwner&& other)           { ptr = other.ptr; other.ptr = nullptr; return *this; }

        ~MemOwner() { if (ptr != nullptr) { ptr->~T(); blocksAsBits.Flip(blockBitIdx); } }
    };


    template<class T>
    inline auto ClaimBlock()
    {
        struct A { std::size_t allocIdx; std::size_t allocBitBegin; };

        constexpr auto a = []() constexpr -> A {
            std::size_t allocBitBegin = 0;
            for(std::size_t i=0; i<ALLOCS_COUNT; ++i) 
            {
                if (allocs[i].blockSize >= sizeof(T)) //assumes sorted
                    return { i, allocBitBegin };
                allocBitBegin += allocs[i].blockCount;
            }
            return {};
        }();

        const auto freeBlock = blocksAsBits.FindFirstFreeBit(a.allocBitBegin);
        constexpr auto blockSize = allocs[a.allocIdx].blockSize;
        blocksAsBits.Flip(freeBlock);
        auto* ptr = allocPtrs[a.allocIdx] + (freeBlock * blockSize);

        LOG((void*)ptr);

        return MemOwner<T> { (T*)ptr, freeBlock };
    }

    




}