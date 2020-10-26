//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"
#include "dbg/Assert.hpp"

//better usable for unions and serialization
//TODO: use for vulkan too
//distinguish between byte and plain array

namespace rpg::com {

#define FOR_SIMPLE_ARRAY(arr, i) for(idx_t i = 0; i < arr.count; ++i)

template<class T, auto N>
struct SimpleArray
{
    //? data
    
    static constexpr auto COUNT_MAX = N;
    T data [N];
    idx_t count = 0;

    //? access

    auto& operator[](auto const idx)       
    { 
        dbg::Assert(idx >= 0 && idx < count, "simple array access out of bounds"); 
        return data[idx]; 
    }
    auto& operator[](auto const idx) const 
    { 
        dbg::Assert(idx >= 0 && idx < count, "simple array access out of bounds"); 
        return data[idx]; 
    }

    //? modification

    void Append(constructible_with auto const&... args)
    {
        dbg::Assert(count <= N, "simple array exhausted");
        data[count] = { args... };
        ++count;
    }

    void Clear()
    {
        count = 0;
    }
};

}//ns