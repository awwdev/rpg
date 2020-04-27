#pragma once
#include <numeric>
#include <cstdint>
#include <type_traits>

namespace mini
{
    //shorthand fixed sizes

    using u64 = std::uint64_t;
    using i64 = std::int64_t;
    using u32 = std::uint32_t;
    using i32 = std::int32_t;
    using u16 = std::uint16_t;
    using i16 = std::int16_t;
    using u8 = std::uint8_t;
    using i8 = std::int8_t;

    using f64 = double;
    using f32 = float;

    constexpr auto u16max = std::numeric_limits<u16>::max();
    constexpr auto u32max = std::numeric_limits<u32>::max();


    template<class T>
    using IsArithmetic = std::enable_if_t<std::is_arithmetic_v<T>>;

    template<class T>
    using IsFloating = std::enable_if_t<std::is_floating_point_v<T>>;

    template<class T>
    using IsIntegral = std::enable_if_t<std::is_integral_v<T> || std::is_enum_v<T>>;
    
}