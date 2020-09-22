#pragma once
#include "Common/Types.hpp"
#include "Debug/Assert.hpp"
#include "Debug/Logger.hpp"

#include <cstddef>
#include <iostream>

namespace rpg::com {

#define FOR_ARRAY(arr, i) for(idx_t i = 0; i < arr.Count(); ++i)

constexpr bool DO_ARRAY_ASSERTS = true;


template<class T, auto N>
struct Array
{
    static constexpr auto COUNT_MAX  = (idx_t) N;
    static constexpr auto BYTE_COUNT = (idx_t) N * sizeof(T);
    using TYPE = T;

    //? RAII
    Array() : count {0} {} //leave bytes array uninit
    explicit Array(std::common_with<T> auto&&... elements) : Array()
    {
        const auto Emplace = [this](T&& element) {
            ArrayAssert(count + 1 <= COUNT_MAX, "array exhausted");
            PlacementNew(std::forward<T>(element));
        };
        (Emplace(static_cast<T>(elements)), ...);
    }

    ~Array()
    {
        Clear();
    }

    void Clear(idx_t beginIdx = 0)
    {
        if constexpr (!std::is_trivial_v<T>) {
            while(count > beginIdx) {
                this->operator[](count - 1).~T(); // avoid trigger ArrayAssert
                --count;
            }
        }
        else count = beginIdx;        
    }

    //? APPEND
    auto Append(auto&&... args) -> T&
    {
        ArrayAssert(count + 1 <= COUNT_MAX, "array exhausted");
        return *PlacementNew(std::forward<decltype(args)>(args)...);
    }

    template<class OTHER_T, auto OTHER_N>
    void AppendArray(const Array<OTHER_T, OTHER_N>& other)
    {
        ArrayAssert(count + other.Count() <= COUNT_MAX, "array exhausted");
        FOR_ARRAY(other, i)
            PlacementNew(static_cast<T>(other[i]));
    }

    template<class OTHER_T, auto OTHER_N>
    void AppendArray(const OTHER_T (&arr)[OTHER_N])
    {
        ArrayAssert(count + OTHER_N <= COUNT_MAX, "array exhausted");
        FOR_CARRAY(arr, i)
            PlacementNew(static_cast<T>(arr[i]));
    }

    //implement (ptr, count) version when needed

    //? ACCESS
    T& operator[](idx_t idx) 
    { 
        ArrayAssert(idx >= 0 && idx < count, "array access out of bounds");
        return reinterpret_cast<T*> (bytes)[idx]; 
    } 
    T const& operator[](idx_t idx) const 
    { 
        ArrayAssert(idx >= 0 && idx < count, "array access out of bounds");
        return reinterpret_cast<T const*>(bytes)[idx];
    }

    auto     Count() const { return count; }
    bool     Empty() const { return count == 0; }

    T*       Data()        { return &this->operator[](0); }
    T const* Data() const  { return &this->operator[](0); }
    T&       Last()        { return this->operator[](count - 1); }
    T const& Last() const {  return this->operator[](count - 1); }

    auto CurrentSize() const { return sizeof(T) * count; }

    //? HELPER
    T* Contains(auto&& element) //allows for custom operator==
    {
        FOR_ARRAY((*this), i) {
            if (this->operator[](i) == element)
                return &(this->operator[](i));
        }
        return nullptr;
    }

private:
    //? INTERNAL HELPER 
    auto PlacementNew(auto&&... args) -> T*
    {
        auto address = &this->operator[](count++);
        return new(address) T { std::forward<decltype(args)>(args)... };
    }

    void ArrayAssert(const bool expr, chars_t msg) const 
    {
        if constexpr (!DO_ARRAY_ASSERTS) return;
        dbg::Assert(expr, msg);
    }

    //? DATA
    alignas(alignof(T)) std::byte bytes [BYTE_COUNT]; //uninit
    idx_t count;
};

//? CTAD
template<typename T, typename...Ts>
Array(T, Ts...) -> Array<T, sizeof...(Ts) + 1>;


template<class T, auto N>
void PrintArray(const Array<T, N>& arr)
{
    FOR_ARRAY(arr, i)
        dbg::LogInfo(arr[i]);
}

}//ns