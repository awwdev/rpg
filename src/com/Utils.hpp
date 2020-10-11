//https://github.com/awwdev

#pragma once
#include "dbg/Assert.hpp"

#include <numeric>
#include <cstdint>
#include <type_traits>
#include <concepts>
#undef max

namespace rpg {
    
//? fixed-size types

using u64 = std::uint64_t;
using s64 = std::int64_t;
using u32 = std::uint32_t;
using s32 = std::int32_t;
using u16 = std::uint16_t;
using s16 = std::int16_t;
using u8  = std::uint8_t;
using s8  = std::int8_t;

using f64 = double;
using f32 = float;

constexpr auto u16max = std::numeric_limits<u16>::max();
constexpr auto u32max = std::numeric_limits<u32>::max();
constexpr auto f32max = std::numeric_limits<f32>::max();

using idx_t   = s32; //default index type
using chars_t = char const* const;

//? concepts

template<class T>
concept as_arithmetic = std::is_arithmetic_v<T>;

template<typename T, typename... Ts>
concept is_constructible_with = requires (Ts&&... args)
{
    T { std::forward<Ts>(args)... };
};

template<typename T>
concept as_enum_integral = std::is_enum_v<T> || std::is_integral_v<T>;

//? array

template<class T, idx_t N>
inline constexpr idx_t ArrayCount(const T (&arr)[N])
{
    return N;
}

#define FOR_C_ARRAY(arr, i) for(idx_t i = 0; i < ArrayCount(arr); ++i)

//? serialization

namespace com
{
    template<typename T, auto N>
    void WriteBinaryFile_C_Array(chars_t path, T const (&arr) [N])
    {
        auto file = std::ofstream(path, std::ios::binary);
        dbg::Assert(file.is_open(), "[IO] cannot open file");
        file.write(reinterpret_cast<char const*>(arr), N * sizeof(T));
    }

    template<typename T, auto N>
    void ReadBinaryFile_C_Array(chars_t path, T (&arr) [N])
    {
        auto file = std::ifstream(path, std::ios::binary);
        dbg::Assert(file.is_open(), "[IO] cannot open file");
        file.read(reinterpret_cast<char*>(arr), N * sizeof(T));
    }
}

}//ns