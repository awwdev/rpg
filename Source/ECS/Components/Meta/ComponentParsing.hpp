//https://github.com/awwdev

#pragma once
#include "com/Container/StringMap.hpp"
#include "dbg/Assert.hpp"
#include "com/Algorithms.hpp"

namespace rpg::ecs {

constexpr auto COMPONENT_DATA_COUNT_MAX = 20; 
constexpr auto COMPONENT_MAX_STR_LEN = 100;
constexpr auto IDENTIFIER_START = 2;

using ComponentDataString = com::String<COMPONENT_MAX_STR_LEN>;
struct ComponentDataStringPair
{
    ComponentDataString key;
    ComponentDataString val;
};

using ComponentDataStringPairs = com::Array<ComponentDataStringPair, ecs::COMPONENT_DATA_COUNT_MAX>;

inline const auto ConvertToComponentDataStringPair(chars_t str)
{
    using namespace ecs;

    const char* beg = &str[IDENTIFIER_START];
    const char* end = [&, end = beg]() mutable {
        while(*end != '=' && (end - beg) < COMPONENT_MAX_STR_LEN) 
            ++end;
        dbg::Assert((end - beg) < COMPONENT_MAX_STR_LEN, "component data has no assignment operator");
        return end;
    }();
    const auto len  = com::StrLen(str);
    const auto len1 = (idx_t)(end - beg) - 1; //rm whitspace
    const auto len2 = (idx_t)(&str[len-1] - (end + 1));

    return ComponentDataStringPair { { &str[IDENTIFIER_START], len1 }, { end + 2, len2 } };;
}

template<class T>
T ParseComponentData(const com::String<COMPONENT_MAX_STR_LEN>& str)
{
    T data {};

    if constexpr(std::is_same_v<T, com::Vec3f>)
    {
        idx_t comma = 0;
        idx_t x = 0;
        for(idx_t i = 0; i < str.length && x < 3; ++i){
            if (str[i] == ',' || i == str.length - 1){
                data[0][x] = (f32)std::atof(str.data + comma);
                comma = i + 1;
                ++x;
            }  
        }
    }
    else
    {
        dbg::Assert(false, "no impl yet");
    }

    return data;
}

}//NS