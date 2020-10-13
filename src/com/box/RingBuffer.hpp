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

    void Add(T const& pElement)
    {
        auto& element = this->operator[](count);
        if constexpr(std::is_array_v<T>)
        {
            FOR_C_ARRAY(element, i)
                element[i] = pElement[i];
        }
        else
        {
            element = pElement;
        }
        ++count;
    }

    T const& Pop()
    {
        --count;
        auto& element = this->operator[](count);
        return element;
    }

private:

    T data [N];
    size_t count = 0;
};

}//ns