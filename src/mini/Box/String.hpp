//https://github.com/awwdev

#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Debug/Assert.hpp"
#include "mini/Debug/Logger.hpp"
#include <iostream>

//! warning this class supports 1 byte chars only for now

namespace mini::box {

constexpr bool USE_STRING_ASSERTS = true;

constexpr void StringAssert(const bool condition, chars_t msg = "string assertion failed")
{
    if (std::is_constant_evaluated())
        return;

    if constexpr(USE_STRING_ASSERTS) {
        if (condition == false){
            ERR(msg);
            DEBUG_BREAK();
        }
    }
}

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

    template<idx_t N>
    constexpr void Append(const String<N, CHAR_T>& str)
    {
        StringAssert(count + str.count - 1 <= CAPACITY, "str capacity exhausted");
        std::memcpy(&data[count-1], str.data, str.count * sizeof(CHAR_T));
        count = count + str.count - 1;
    }

    template<idx_t N>
    constexpr void Append(const CHAR_T (&arr)[N])
    {
        StringAssert(count + N - 1 <= CAPACITY, "str capacity exhausted");
        std::memcpy(&data[count-1], arr, N * sizeof(CHAR_T));
        count = count + N - 1;
    }

    template<class PTR, typename = IsNoArray<PTR>, typename = IsPointer<PTR>>
    constexpr void Append(const PTR ptr, const idx_t countNotLength) //!has to be CHAR_T
    {
        StringAssert(count + countNotLength - 1 <= CAPACITY, "str capacity exhausted");
        std::memcpy(&data[count-1], ptr, countNotLength * sizeof(CHAR_T));
        count = count + countNotLength - 1;
    }

    constexpr void Append(const CHAR_T ch)
    {
        StringAssert(count + 1 <= CAPACITY, "str capacity exhausted");
        data[count] = ch;
        ++count;
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