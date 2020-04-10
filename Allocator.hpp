#pragma once
#include "MiniSTL/Types.hpp"
#include "MiniSTL/Array.hpp"

namespace mini::mem
{
    struct IBlock 
    { 
        std::byte* begin; 
        std::byte* end; 
        inline std::size_t Size() const { return end - begin; }
    };
    template<class T> struct Block : IBlock { T* const ptr; };
    
    extern std::byte* base;
    extern container::Array<IBlock, 1024> freeBlocks;
    extern container::Array<IBlock, 1024> usedBlocks;


    inline void Allocate(const std::size_t bytes)
    {
        base = new std::byte[bytes];
        freeBlocks.EmplaceBack(base, base + bytes);
    }

    template<class T, class... Args>
    T* Claim(Args&&... args)
    {
        //get candidates
        container::Array<IBlock*, 1024> bigEnough;
        FOR_ARRAY(freeBlocks, i)
        {
            if (sizeof(T) == freeBlocks[i].Size())
            {
                usedBlocks.EmplaceBack(freeBlocks[i]);
                freeBlocks.RemoveUnordered(i);
                return new(usedBlocks.Last().begin) T(std::forward<Args>(args)...);
            }
            else if (sizeof(T) < freeBlocks[i].Size())
            {
                bigEnough.EmplaceBack(&freeBlocks[i]);
            }
        }
        //get smallest
        std::size_t minSize = bigEnough[0]->Size();
        IBlock* minBlock = bigEnough[0];
        FOR_ARRAY(bigEnough, i) //start by i=1
        {
            if (bigEnough[i]->Size() < minSize)
            {
                minSize  = bigEnough[i]->Size();
                minBlock = bigEnough[i];
            }
        }
        if (minBlock == nullptr)
            return nullptr;

        //chop free block
        usedBlocks.EmplaceBack(minBlock->begin, minBlock->begin + sizeof(T));
        minBlock->begin += sizeof(T);
        return new(usedBlocks.Last().begin) T(std::forward<Args>(args)...);
    }

    template<class T>
    void Free(T* const ptr)
    {
        //loop all used blocks and see where ptr==begin
    }
}