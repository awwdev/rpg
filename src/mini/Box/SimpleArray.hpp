//https://github.com/awwdev

#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Debug/Assert.hpp"
#include "mini/Debug/Logger.hpp"

//currently used a lot by vk impl
//super simple wrapper to keep size data around

namespace mini::box
{
    #define FOR_SIMPLE_ARRAY(arr, i) for(uint32_t i = 0; i < arr.count; ++i)

    template<class T, uint32_t N>
    struct SimpleArray
    {
        SimpleArray(const uint32_t pCount = 0) : count { pCount } { ; }

        uint32_t count;
        T data[N];

        T&       operator[](const uint32_t i)       { return data[i]; }
        const T& operator[](const uint32_t i) const { return data[i]; }
    };

}//ns