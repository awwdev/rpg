//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"
#include "dbg/Assert.hpp"

namespace rpg::com {

constexpr bool USE_SIMPLE_ARRAY_ASSERTS = true;

#define FOR_SIMPLE_ARRAY(arr, i) \
for(decltype(arr.count) i = 0; i < arr.count; ++i)

template<typename T, auto N>
struct SimpleArray
{
    //meta
    using ELEMENT_T = T;
    using INDEX_T = decltype(N);
    static constexpr auto COUNT_MAX = N;
    static constexpr auto BYTES_MAX = N * sizeof(T);

    //data
    T data [N];
    INDEX_T count = 0;

    //? access
    auto& operator[](INDEX_T const idx)       
    { 
        SimpleArrayAssert(idx >= 0 && idx < count, "simple array access out of bounds"); 
        return data[idx]; 
    }
    auto& operator[](INDEX_T const idx) const 
    { 
        SimpleArrayAssert(idx >= 0 && idx < count, "simple array access out of bounds"); 
        return data[idx]; 
    }

    //? modification

    void Append(constructible_with auto const&... args)
    {
        SimpleArrayAssert(count <= N, "simple array exhausted");
        data[count] = { args... };
        ++count;
    }

    void ResetCount()
    {
        count = 0;
    }

private:

    void SimpleArrayAssert(bool const expr, chars_t msg) const 
    {
        if constexpr (!USE_SIMPLE_ARRAY_ASSERTS) return;
        dbg::Assert(expr, msg);
    }

};

template<typename T, uint32_t N>
using SimpleArrayVk = SimpleArray<T, N>;

}//ns