//https://github.com/awwdev

#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Debug/Assert.hpp"
#include "mini/Debug/Logger.hpp"

#include <iostream>
#include <charconv>

//! warning this class supports 1 byte chars only for now

namespace mini::box {

constexpr bool USE_STRING_ASSERTS = true;

constexpr void StringAssert(const bool condition, chars_t msg = "string assertion failed")
{
    if (std::is_constant_evaluated())
        return;

    if constexpr(USE_STRING_ASSERTS) {
        if (condition == false){
            dbg::LogError(msg);
            DEBUG_BREAK();
        }
    }
}

#define FOR_STRING(str, i) for(idx_t i = 0; i < str.Length(); ++i)

template<idx_t CAPACITY_T, typename CHAR_T = char>
struct String
{
    //? DATA

    using DATA_T = CHAR_T;
    static constexpr idx_t CAPACITY = (idx_t)CAPACITY_T;

    CHAR_T data [CAPACITY] { 0 };
    idx_t  count = 1; //! does include \0

    constexpr       CHAR_T& operator[] (const idx_t i)       { return data[i]; }
    constexpr const CHAR_T& operator[] (const idx_t i) const { return data[i]; }

    constexpr idx_t Length() const { return count - 1;  }
    constexpr bool  Empty()  const { return count <= 1; }

    //? CTOR

    constexpr String() = default;

    template<idx_t N>
    constexpr String(const CHAR_T (&arr)[N])
        : data  {}
        , count { N }
    {
        StringAssert(N <= CAPACITY, "str capacity exhausted");
        FOR_CARRAY(arr, i)
            data[i] = arr[i];
    }

    template<idx_t N>
    constexpr String(const String<N, CHAR_T>& str)
    {
        Clear(); 
        Append(str);
    }

    template<idx_t N>
    constexpr String& operator=(const String<N, CHAR_T>& str)
    {
        Clear(); 
        Append(str);
        return *this;
    }

    //? MODIFICATION 

    constexpr void Clear()
    {
        count = 1;
        data[0] = '\0';
    }

    template<idx_t N> constexpr void Append(const String<N, CHAR_T>& str)   { Append(str.data, str.count); }
    template<idx_t N> constexpr void Append(const CHAR_T (&arr)[N])         { Append(arr, N); }
    constexpr void Append(const CHAR_T ch)                                  { Append(&ch, 2); }
    
    template<class PTR, typename = IsNoArray<PTR>, typename = IsPointer<PTR>>
    constexpr void Append(const PTR ptr, const idx_t countNotLength) //!has to be CHAR_T
    {
        StringAssert(count + countNotLength - 1 <= CAPACITY, "str capacity exhausted");
        std::memcpy(&data[count-1], ptr, countNotLength * sizeof(CHAR_T));
        count = count + countNotLength - 1;
    }

    //allow fundemental types
    template<class T>//, typename = std::enable_if_t<!std::is_array_v<std::remove_cvref_t<T>>>>
    void Append(T&& fundemental)
    {
        char buffer[30] {};
        std::to_chars(buffer, &buffer[30], fundemental);
        Append(&buffer, (idx_t)std::strlen(buffer) + 1);
    }

    constexpr void Pop()
    {
        if (Empty()) return;
        data[count-1] = '\0';
        --count;
    }

};

template<class... STRINGS>
constexpr auto ConcatStrings(const STRINGS&... strs)
{
    static_assert(sizeof...(STRINGS) > 0);

    const auto TOTAL_COUNT = []() constexpr {
        idx_t count = 0;
        ((count += STRINGS::CAPACITY), ...);
        return count;
    }();

    String<TOTAL_COUNT> str;
    ((str.Append(strs.data, strs.count)), ...);

    return str;
}

template<auto N>
std::ostream& operator<<(std::ostream& os, const String<N>& str)
{
    return (os << str.data);
}

}//ns