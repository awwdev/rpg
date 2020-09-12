//https://github.com/awwdev

#pragma once
#include "Common/Types.hpp"
#include "Debug/Assert.hpp"
#include "Common/Algorithms.hpp"

#include <iostream>
#include <charconv>
#include <string>

namespace rpg::com {

constexpr bool USE_STRING_ASSERTS = true;

constexpr void StringAssert(const bool condition, chars_t msg = "string assertion failed")
{
    if (std::is_constant_evaluated())
        return;

    if constexpr(USE_STRING_ASSERTS) {
        if (condition == false){
            dbg::LogError(msg);
            dbg::DebugBreak();
        }
    }
}

#define FOR_STRING(str, i) for(idx_t i = 0; i < str.length; ++i)

//not a wide string
template<idx_t CAPACITY_T>
struct String
{
    //? DATA
    using DATA_T = char;
    static constexpr idx_t CAPACITY = (idx_t)CAPACITY_T;

    DATA_T data [CAPACITY] { '\0' };
    idx_t  length = 0;

          DATA_T& operator[] (const idx_t i)       { return data[i]; }
    const DATA_T& operator[] (const idx_t i) const { return data[i]; }

    bool Empty()  const { return length == 0; }
    bool Full()   const { return length >= (CAPACITY-1); } 

    //? CTOR

    String() = default;
    template<idx_t N> String(const DATA_T (&arr)[N])  { Append(arr, N); }
    template<idx_t N> String(const String<N>& str)    { Append(str.data, str.length); }
    String(chars_t ptr)                               { Append(ptr, com::StrLen(ptr)); }
    String(chars_t ptr, const idx_t range)            { Append(ptr, range); }

    template<class T, typename = std::enable_if_t<!std::is_same_v<std::decay_t<T>, DATA_T>>>
    String(T&& arithemtic) { Append(std::forward<T>(arithemtic)); }

    //? MODIFICATION 

    void Clear()
    {
        length  = 0;
        data[0] = '\0';
    }

    template<idx_t N> void Append(const String<N>& str)   { Append(str.data, str.length); }
    template<idx_t N> void Append(const DATA_T (&arr)[N]) { Append(arr, N); }
    void Append(const DATA_T ch)                          { Append(&ch, 1); }
        
    void Append(const DATA_T* const ptr, const idx_t pLength)
    {
        StringAssert(length + pLength < CAPACITY, "str capacity exhausted");
        std::memcpy(&data[length], ptr, pLength * sizeof(DATA_T));
        length += pLength;
        data[length + 1] = '\0';       
    }

    template<class T, typename = std::enable_if_t<!std::is_same_v<std::decay_t<T>, DATA_T>>>
    void Append(T&& t)
    {
        //charsconv is not implemented in gcc STL
        const auto str = std::to_string(t); 
        Append(str.c_str(), (idx_t)str.length());
    }

    constexpr void Pop()
    {
        if (Empty()) return;
        --length;
        data[length] = '\0';
    }

    //? COMPARSION

    constexpr bool operator==(const String& other) const
    {
        return std::strcmp(this->data, other.data) == 0;
    }

    constexpr bool operator==(chars_t other) const
    {
        return std::strcmp(this->data, other) == 0;
    }

};

template<class... STRINGS>
auto ConcatStrings(const STRINGS&... strs)
{
    static_assert(sizeof...(STRINGS) > 0);

    constexpr auto TOTAL_CAPACITY = []() constexpr {
        idx_t cap = 0;
        ((cap += STRINGS::CAPACITY), ...);
        return cap;
    }();

    String<TOTAL_CAPACITY> str;
    (str.Append(strs), ...);

    return str;
}

template<auto N>
std::ostream& operator<<(std::ostream& os, const String<N>& str)
{
    return (os << str.data);
}

}//ns