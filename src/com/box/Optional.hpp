//https://github.com/awwdev

#pragma once

namespace rpg::com {
    
//COPY
template<class T>
struct Optional
{
    Optional()                : value {},         hasValue { false } {}
    Optional(T const& pValue) : value { pValue }, hasValue { true  } {}

    operator bool() const { return hasValue; }
    //operator T()    const { return value; }

    T*       operator->()       { return &value; }
    const T* operator->() const { return &value; }

    T&       operator* ()       { return value; }
    T const& operator* () const { return value; }

private:
    T    value;
    bool hasValue;
};

}//ns