//https://github.com/awwdev

#pragma once

namespace rpg::com {
    
template<class T>
struct Optional
{
    Optional()       : t{},     hasValue { false } {}
    Optional(T&& pT) : t{ pT }, hasValue { true  } {}
    operator bool() const { return hasValue; }
    T*       operator-> ()        { return &t; }
    const T* operator-> ()  const { return &t; }

    T t;
    bool hasValue;
};

}//ns