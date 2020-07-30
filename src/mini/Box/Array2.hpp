//https://github.com/awwdev

#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Debug/Assert.hpp"
#include "mini/Debug/Logger.hpp"

//TODO: rework logger!
//TODO: placement new fn
//TODO: just alway use u32 for index

namespace mini::box2
{
    constexpr auto USE_ARRAY_ASSERTS = true;

    inline void Assert(const bool condition, chars_t msg = "assertion failed")
    {
        if constexpr(USE_ARRAY_ASSERTS) {
            if (condition == false){
                ERR(msg);
                DEBUG_BREAK();
            }
        }
    }

    #define FOR_ARRAY2(arr, i) for(decltype(arr.count) i = 0; i < arr.count; ++i)

    template<class T, auto N>
    struct Array
    {
        using TYPE = T;
        using IDX  = decltype(N);

        IDX count = 0;
        static constexpr auto CAPACITY = N;
        alignas(T) u8 bytes[sizeof(T) * N]; //don't init

        //? ACCESS

        T& operator[](const IDX idx) 
        { 
            Assert(idx < count); 
            return reinterpret_cast<T&>(bytes[sizeof(T) * idx]); 
        }

        const T& operator[](const IDX idx) const 
        { 
            Assert(idx < count); 
            return reinterpret_cast<const T&>(bytes[sizeof(T) * idx]); 
        }

        T& Last()       
        { 
            this->operator[count - 1]; 
        }
        const T& Last() const 
        { 
            this->operator[count - 1];
        }

        //? COUNT MODIFICATION

        template<class... ARGS>
        T* Append(ARGS&&... args)
        {
            ++count;
            Assert(count <= CAPACITY, "array capacity exhausted");
            return new(&bytes[sizeof(T) * (count - 1)]) T { std::forward<ARGS>(args)... };
        }

        template<auto OTHER_N>
        void AppendArray(const Array<T, OTHER_N>& other)
        {
            Assert((count + other.count) <= CAPACITY, "array capacity exhausted");
            FOR_ARRAY2(other, i) {
                ++count;
                new(&bytes[sizeof(T) * (count - 1)]) T { other[i] };
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
            FOR_ARRAY2((*this), i) {
                if (this->operator[](i) == element)
                    return &(this->operator[](i));
            }
            return nullptr;
        }

        bool Empty() const { return count == 0; }
        std::size_t CurrentSize() const { return sizeof(T) * count; }
    };

}//ns