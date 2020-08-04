//https://github.com/awwdev

#pragma once

namespace mini::box {
    
template<class T>
struct Optional
{
    T t;
    bool hasValue;
    Optional()       : hasValue { false }          {}
    Optional(T&& pT) : hasValue { true  }, t{ pT } {}
    operator bool() const { return hasValue; }
    T*       operator-> ()        { return &t; }
    const T* operator-> ()  const { return &t; }
};

}//ns