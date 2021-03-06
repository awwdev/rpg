//https://github.com/awwdev

#pragma once
#include <numeric>
#include <cstdint>
#include <type_traits>
#include <concepts>
#include "utils/UndefWindows.hpp"

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
constexpr auto f32min = std::numeric_limits<f32>::min();
constexpr auto f32max = std::numeric_limits<f32>::max();

using idx_t   = s32; //default index type
using chars_t = char const* const;

//? concepts

template<typename T>
concept as_arithmetic = std::is_arithmetic_v<T>;

template<typename T, typename... Ts>
concept is_constructible_with = requires (Ts&&... args)
{
    T { std::forward<Ts>(args)... };
};

template<typename T>
concept as_enum_integral = std::is_enum_v<T> || std::is_integral_v<T>;

template<typename T>
concept as_advancable_enum = requires {
    std::is_enum_v<T>;
    T::ENUM_END;
};

template<typename T, typename... Ts>
concept constructible_with = std::is_trivially_constructible_v<T, Ts...> || std::is_constructible_v<T, Ts...>;

//? array

template<typename T, unsigned N>
constexpr auto array_extent(const T (&arr)[N])
{
    return static_cast<idx_t>(N);
}

#define FOR_C_ARRAY(arr, i) for(auto i = 0; i < array_extent(arr); ++i)

}//ns