//https://github.com/awwdev

#pragma once
#include "com/Utils.hpp"

namespace rpg::com {

template<typename T, auto N>
struct RingBuffer
{
    T data [N] {}; //init needed for union
    s32 idx   = -1;
    s32 count =  0;
    s32 maxIdx = idx;

    auto& operator[](auto const i)       { return data[i % N]; }
    auto& operator[](auto const i) const { return data[i % N]; }

    void Append(T const& element)
    {
        ++idx;
        ++count;
        maxIdx = idx;
        com::Clamp(count, 0, N);
        operator[](idx) = element; //TODO: array
    }

    T* StepBackwards()
    {
        if (idx - 1 < -1 || count - 1 < 0) return nullptr;
        --count;
        return &operator[](idx--); //return current and then decrement
    }

    T* StepForwards()
    {
        if (idx + 1 > maxIdx) return nullptr;
        ++count;
        return &operator[](++idx);
    }  

    auto& Current()       { return operator[](idx); }
    auto& Current() const { return operator[](idx); }

};

} //ns