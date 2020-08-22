//https://github.com/awwdev

#pragma once
#include "mini/Box/StringMap.hpp"
#include "mini/Debug/Assert.hpp"

namespace mini::ecs {

constexpr auto COMPONENT_DATA_COUNT_MAX = 20; 
constexpr auto COMPONENT_MAX_STR_LEN = 100;
constexpr auto IDENTIFIER_START = 2;

using ComponentDataString = box::String<COMPONENT_MAX_STR_LEN>;
struct ComponentDataStringPair
{
    ComponentDataString key;
    ComponentDataString val;
};

using ComponentDataStringPairs = box::Array<ComponentDataStringPair, ecs::COMPONENT_DATA_COUNT_MAX>;

inline const auto ConvertToComponentDataStringPair(chars_t str)
{
    using namespace ecs;

    const char* beg = &str[IDENTIFIER_START];
    const char* end = [&, end = beg]() mutable {
        while(*end != '=' && std::distance(beg, end) < COMPONENT_MAX_STR_LEN) 
            ++end;
        dbg::Assert(std::distance(beg, end) < COMPONENT_MAX_STR_LEN, "component data has no assignment operator");
        return end;
    }();
    const auto len  = std::strlen(str);
    const auto len1 = (idx_t)std::distance(beg, end) - 1; //rm whitspace
    const auto len2 = (idx_t)std::distance(end + 1, &str[len-1]);

    return ComponentDataStringPair { { &str[IDENTIFIER_START], len1 }, { end + 2, len2 } };;
}

template<class T>
T ParseComponentData(const box::String<COMPONENT_MAX_STR_LEN>& str)
{
    T data {};

    if constexpr(std::is_same_v<T, utils::Vec3f>)
    {
        idx_t comma = 0;
        idx_t x = 0;
        for(idx_t i = 0; i < str.Length() && x < 3; ++i){
            if (str[i] == ',' || i == str.Length() - 1){
                data[0][x] = (f32)std::atof(str.data + comma);
                comma = i + 1;
                ++x;
            }  
        }
    }
    else
    {
        static_assert(false, "no impl yet");
    }

    return data;
}

}//NS