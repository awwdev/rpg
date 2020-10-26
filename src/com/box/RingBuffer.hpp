//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"

namespace rpg::com {

//cannot store arrays, if wanted put them into struct

template<typename T, auto N>
struct RingBuffer
{
    T data [N];

    s32 currentIdx = -1;
    s32 count  = 0;
    s32 maxIdx = currentIdx;

    auto& operator[](auto const i)       { return data[i % N]; }
    auto& operator[](auto const i) const { return data[i % N]; }

    void Append(T const& element)
    {
        currentIdx++;
        count++;
        maxIdx = currentIdx;
        com::Clamp(count, 0, N);
        operator[](currentIdx) = element;
    }

    T* StepBackwards()
    {
        if (currentIdx - 1 < -1 || count - 1 < 0) return nullptr;
        count--;
        currentIdx--;
        return &operator[](currentIdx+1);
    }

    T* StepForwards()
    {
        if (currentIdx + 1 > maxIdx) return nullptr;
        count++;
        currentIdx++;
        return &operator[](currentIdx);
    }  

};

} //ns