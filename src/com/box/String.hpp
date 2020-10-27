//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"
#include "dbg/Assert.hpp"
#include "com/utils/Utils.hpp"

#include <iostream>
#include <charconv>
#include <string>
#include <concepts>

namespace rpg::com {

constexpr bool USE_STRING_ASSERTS = true;

constexpr void StringAssert(const bool condition, chars_t msg = "string assertion failed")
{
    if (std::is_constant_evaluated())
        return;

    if constexpr(USE_STRING_ASSERTS) {
        dbg::Assert(condition, msg);
    }
}

#define FOR_STRING(str, i) for(idx_t i = 0; i < str.Length(); ++i)

//not a wide string
template<idx_t CAPACITY_T>
struct String
{
    //? DATA
    using DATA_T = char;
    static constexpr idx_t CAPACITY = (idx_t)CAPACITY_T;

private:
    DATA_T cstr [CAPACITY];
    idx_t  length = 0;

public:

          DATA_T& operator[] (const idx_t i)       { StringAssert(length > 0); return cstr[i]; }
    const DATA_T& operator[] (const idx_t i) const { StringAssert(length > 0); return cstr[i]; }

    bool Empty()  const { return length == 0; }
    bool Full()   const { return length >= (CAPACITY-1); } 
    
    DATA_T*       Data()       { StringAssert(length > 0); return cstr; }
    DATA_T const* Data() const { StringAssert(length > 0); return cstr; }

    idx_t Length() const { return length; }

    //? CTOR

    String() = default;
    template<idx_t N> String(const DATA_T (&arr)[N])  { AppendArray(arr); }
    template<idx_t N> String(const String<N>& str)    { AppendString(str); }
    String(chars_t ptr)                               { Append(ptr, com::StrLen(ptr)); }
    String(chars_t ptr, const idx_t range)            { Append(ptr, range); }

    //need of custom ones, since on copy the compilier WILL NOT ADD my null-termination
    String(const String& other)         { AppendString(other); }
    String(String&& other)              { AppendString(other); }
    void operator=(const String& other) { Clear(); AppendString(other); }
    void operator=(String&& other)      { Clear(); AppendString(other); }

    String          (as_arithmetic auto const& arithemtic) { AppendArithemtic(arithemtic); }
    void   operator=(as_arithmetic auto const& arithemtic) { Clear(); AppendArithemtic(arithemtic); }

    //? MODIFICATION 

    void Clear()
    {
        length = 0;
    }

    template<idx_t N> void AppendString(const String<N>& str)   { Append(str.Data(), str.Length()); }
    template<idx_t N> void AppendArray(const DATA_T (&arr)[N])  { Append(arr, N - 1); }
    void Append(const DATA_T ch)                                { Append(&ch, 1); }
        
    void Append(const DATA_T* const ptr, const idx_t pLength)
    {
        StringAssert(length + pLength < CAPACITY, "str capacity exhausted");
        std::memcpy(&cstr[length], ptr, pLength * sizeof(DATA_T));
        length += pLength;
        cstr[length] = '\0';       
    }

    template<typename T, typename = std::enable_if_t<!std::is_same_v<std::decay_t<T>, DATA_T>>>
    void AppendArithemtic(T&& t)
    {
        //charsconv is not implemented in gcc STL
        const auto str = std::to_string(t); 
        Append(str.data(), (idx_t)str.length()); //STD string !!
    }

    constexpr void Pop()
    {
        if (Empty()) return;
        --length;
        cstr[length] = '\0';
    }

    //? COMPARSION

    constexpr bool operator==(const String& other) const
    {
        return std::strcmp(this->Data(), other.Data()) == 0;
    }

    constexpr bool operator==(chars_t other) const
    {
        return std::strcmp(this->Data(), other) == 0;
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
    return (os << str.Data());
}

}//ns