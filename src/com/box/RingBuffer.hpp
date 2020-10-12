//https://github.com/awwdev

#pragma once
#include "com/Utils.hpp"
#include "dbg/Assert.hpp"
#include "com/Algorithms.hpp"

namespace rpg::com {

template<typename T, size_t N>
struct RingBuffer
{
    auto& operator[](auto const idx) 
    { 
        return data[idx % N]; 
    }

    auto& operator[](auto const idx) const
    { 
        return data[idx % N]; 
    }

    void Append(T const& element)
    {
        this->operator[](index) = element;
        ++index;
    }

private:
    T data [N];
    size_t index = 0;
};

}//ns