//https://github.com/awwdev

#pragma once

namespace rpg::com {

template<typename T>
struct Optional
{
    bool hasValue;
    T    value;

    Optional()
        : hasValue { false }
        , value    {}
    {}

    Optional(T const& pValue)
        : hasValue { true }
        , value    { pValue }
    {}

    //! watch to not use "auto value = optional", but *optional or optional.value
    //! since it will implicitly convert to bool
    operator bool() const { return hasValue; }

    T*       operator->()       { return &value; }
    const T* operator->() const { return &value; }
    T&       operator* ()       { return  value; }
    T const& operator* () const { return  value; }
};

}//ns