//https://github.com/awwdev

#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Debug/Assert.hpp"
#include "mini/Debug/Logger.hpp"

//currently used a lot by vk impl
//super simple wrapper to keep size data around

namespace mini::box
{
    #define FOR_SIMPLE_ARRAY(arr, i) for(u32 i = 0; i < arr.count; ++i)

    template<class T, u32 N>
    struct VkArray
    {
        static constexpr auto CAPACITY = N;
        VkArray(const u32 pCount = 0) : count { pCount } { ; }

        u32 count;
        T   data[N];

        T&       operator[](const u32 i)       { return data[i]; }
        const T& operator[](const u32 i) const { return data[i]; }

        template<class... CtorArgs>
        void Append(CtorArgs&&... args)
        {
            //TODO: bounds check?
            data[count] = { std::forward<CtorArgs>(args)... };
            ++count;
        }

        void RemoveSwapped(const u32 id)
        {
            data[id] = data[count - 1];
            --count;
        }
    };

    template<class T, auto N>
    struct SimpleArray
    {
        using TYPE = T;
        using IDX  = decltype(N);

        static constexpr IDX CAPACITY = N;

        using    REF  =       T(&)[N];
        using    CREF = const T(&)[N];
        operator REF ()       { return data; }
        operator CREF() const { return data; }

        T&       operator[](const IDX i)       { return data[i]; }
        const T& operator[](const IDX i) const { return data[i]; }

        T data [N];
    };

}//ns