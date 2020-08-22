//https://github.com/awwdev

#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Debug/Assert.hpp"
#include "mini/Debug/Logger.hpp"

namespace mini::box {

constexpr bool USE_ARRAY_ASSERTS = true;

constexpr void ArrayAssert(const bool condition, chars_t msg = "array assertion failed")
{
    if (std::is_constant_evaluated())
        return;

    if constexpr(USE_ARRAY_ASSERTS) {
        if (condition == false){
            dbg::LogError(msg);
            DEBUG_BREAK();
        }
    }
}

#define FOR_ARRAY(arr, i) for(idx_t i = 0; i < arr.count; ++i)

template<class T, auto N>
struct Array
{
    using TYPE = T;

    idx_t count = 0;
    static constexpr idx_t CAPACITY = (idx_t)N;
    alignas(T) u8 bytes[sizeof(T) * CAPACITY]; //don't init

    Array() = default;
    Array(std::initializer_list<T> list) {
        for(auto&& element : list){
            Append(element);
        }
    }

    //? ACCESS

    template<class IDX> T&       operator[](const IDX idx)       { return reinterpret_cast<T&>      (bytes[sizeof(T) * (idx_t)idx]); }
    template<class IDX> const T& operator[](const IDX idx) const { return reinterpret_cast<const T&>(bytes[sizeof(T) * (idx_t)idx]); }

    T&       Last()       { ArrayAssert(count > 0); return this->operator[](count - 1); }
    const T& Last() const { ArrayAssert(count > 0); return this->operator[](count - 1); }
    T*       Data()       { return &(this->operator[](0)); }
    const T* Data() const { return &(this->operator[](0)); }

    //? COUNT MODIFICATION

    template<class... ARGS>
    T* Append(ARGS&&... args)
    {
        ArrayAssert(count < CAPACITY, "array capacity exhausted");
        auto* ptr = PlacementNew(std::forward<ARGS>(args)...);
        ++count;
        return ptr;
    }

    template<auto OTHER_N>
    void AppendArray(const Array<T, OTHER_N>& other)
    {
        ArrayAssert((count + other.count) <= CAPACITY, "array capacity exhausted");
        FOR_ARRAY(other, i) {
            PlacementNew(other[i]);
            ++count;
        }
    }

    void Clear()
    {
        if constexpr(!std::is_trivial_v<T>) {
            while(count > 0)
                this->operator[](--count).~T();
        }
        else count = 0;
    }

    //? HELPER

    template<class E>
    T* Contains(const E& element) //allows for custom operator==
    {
        FOR_ARRAY((*this), i) {
            if (this->operator[](i) == element)
                return &(this->operator[](i));
        }
        return nullptr;
    }

    template<class... ARGS>
    T* PlacementNew(ARGS&&... args)
    {
        return new(&bytes[sizeof(T) * count]) T { std::forward<ARGS>(args)... };
    }

    bool Empty()       const { return count == 0; }
    auto CurrentSize() const { return sizeof(T) * count; }
};

template<class T, auto N>
void PrintArray(const Array<T, N>& arr)
{
    FOR_ARRAY(arr, i){
        std::cout << arr[i] << ',';
    }
    std::cout << '\n';
}

}//ns