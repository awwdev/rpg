//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"
#include "dbg/Assert.hpp"

namespace rpg::com {

auto constexpr USE_OPTIONAL_ASSERTS = true;

template<typename T>
struct Optional
{
    //? constructors

    Optional()
        : hasValue { false }
        , data     { .empty {} }
    {}

    Optional(constructible_with auto const&... args)
        : hasValue { true }
        , data     { .value { args... } }
    {}

    explicit operator bool() const { return hasValue; }
    bool HasValue() const          { return hasValue; }

    //? access

    T*       operator->()       { OptionalAssert(hasValue, "optional access invalid"); return &data.value; }
    const T* operator->() const { OptionalAssert(hasValue, "optional access invalid"); return &data.value; }
    T&       operator* ()       { OptionalAssert(hasValue, "optional access invalid"); return  data.value; }
    T const& operator* () const { OptionalAssert(hasValue, "optional access invalid"); return  data.value; }

private:
    
    //? data

    const bool hasValue;
    const union 
    {
        struct {} empty;
        T value;
    } 
    data;

    //? internal helper

    void OptionalAssert(bool const expr, chars_t msg) const 
    {
        if constexpr (!USE_OPTIONAL_ASSERTS) return;
        dbg::Assert(expr, msg);
    }

};

}//ns