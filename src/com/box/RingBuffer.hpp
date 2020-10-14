//https://github.com/awwdev

#pragma once
#include "com/Utils.hpp"

namespace rpg::com {

template<typename T, auto N>
struct RingBuffer
{
    T data [N] {};
    std::size_t index = 0;

    auto& operator[](auto const idx) 
    {
        return data[idx % N];
    }

    auto& operator[](auto const idx) const
    {
        return data[idx % N];
    }

    auto& StepForward()
    {
        return operator[](index++);
    }

    auto& StepBackward()
    {
        return operator[](--index);
    }

    void Append(T const& pElement)
    {
        auto& element = this->operator[](index);
        if constexpr(std::is_array_v<T>)
        {
            dbg::Assert(false, "not impl yet");
        }
        else
        {
            element = pElement;
        }
        ++index;
    }

};

} //ns