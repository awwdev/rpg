#pragma once
#include "MiniSTL/Types.hpp"
#include "MiniSTL/Container/Array.hpp"
#include <fstream>

namespace mini::mem
{
    struct Block
    {
        std::byte* begin;
        std::byte* end;
        std::byte* aligned;
        inline std::size_t Size() const { return end - begin; }
    };

    inline std::byte* base;
    inline size_t totalSize;
    inline container::Array<Block, 1024> freeBlocks;
    inline container::Array<Block, 1024> usedBlocks;

    template<class T>
    void Free(T* const ptr) //hide this from user
    {
        FOR_ARRAY(usedBlocks, i)
        {
            if (usedBlocks[i].aligned == (std::byte* const)ptr) {
                ptr->~T();
                freeBlocks.Append(usedBlocks[i]);
                usedBlocks.Remove(i);
                return;
            }
        }
    }

    template<class T> struct Owner
    {
        T* const ptr;
        T* operator->() { return ptr; }
        ~Owner()        { Free(ptr); }
    };

    inline void Allocate(const std::size_t bytes)
    {
        base = new std::byte[bytes];
        freeBlocks.Append(base, base + bytes);
        totalSize = bytes;
    }

    inline std::byte* NextAlignedAddress(const std::byte* ptr, const u8 alignment)
    {
        return (std::byte*) (((uintptr_t)ptr + (alignment - 1)) & ~(alignment - 1));
    }

    template<class T, class... Args>
    Owner<T> Claim(Args&&... args)
    {
        //get candidates
        container::Array<Block*, 1024> bigEnough;
        FOR_ARRAY(freeBlocks, i)
        {
            if (sizeof(T) == freeBlocks[i].Size() + alignof(std::max_align_t)) //test
            {
                auto& block = usedBlocks.AppendRtn(freeBlocks[i]);
                block.aligned = NextAlignedAddress(block.begin, alignof(T));
                freeBlocks.Remove(i);
                return { new(block.aligned) T(std::forward<Args>(args)...) };
            }
            else if (sizeof(T) < freeBlocks[i].Size() + alignof(std::max_align_t))
            {
                bigEnough.Append(&freeBlocks[i]);
            }
        }
        //get smallest
        std::size_t minSize = bigEnough[0]->Size();
        Block* minBlock = bigEnough[0];
        FOR_ARRAY(bigEnough, i) //start by i=1
        {
            if (bigEnough[i]->Size() < minSize)
            {
                minSize  = bigEnough[i]->Size();
                minBlock = bigEnough[i];
            }
        }
        if (minBlock == nullptr)
            return { nullptr };

        //chop free block
        const auto aligned = NextAlignedAddress(minBlock->begin, alignof(T));
        auto& block = usedBlocks.AppendRtn(minBlock->begin, aligned + sizeof(T), aligned);
        minBlock->begin = aligned + sizeof(T);
        return { new(block.aligned) T(std::forward<Args>(args)...) };
    }

    inline auto TotalUsed() 
    {
        std::size_t total = 0;
        FOR_ARRAY(usedBlocks, i) { total += usedBlocks[i].Size(); }
        return total;
    }
    inline auto TotalFree() 
    {
        std::size_t total = 0;
        FOR_ARRAY(freeBlocks, i) { total += freeBlocks[i].Size(); }
        return total;
    }
}