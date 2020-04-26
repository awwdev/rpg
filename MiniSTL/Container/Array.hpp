#pragma once
#include "MiniSTL/Types.hpp"
#include "MiniSTL/Debug/Assert.hpp"
#include "MiniSTL/Debug/Profiler.hpp"
#include <compare>

#define FOR_ARRAY(arr, i) for(std::decay_t<decltype(arr)>::COUNT_TYPE i=0; i<arr.Count(); ++i)

//may comment out on release mode:
#define CHECK_INDEX(i, c)       mini::Assert(i<c && i>=0);
#define CHECK_CAPACITY(c, cap)  mini::Assert(c<=cap);

#define ANY_SIZE template<typename A, typename = std::enable_if_t<std::is_same_v<T, typename A::DATA_TYPE>>>


namespace mini::container
{   
    //used for passing only
    template<class T, typename C = u16> class IArray
    {
    protected:
        IArray(T* const buffer, const C capacity)
            : bufferPtr { buffer }
            , CAPACITY  { capacity }
            , count     { 0 }
        {}
        ~IArray()
        {
            Clear();
        }
    public:
        [[nodiscard]] T&       operator[](const C i)        { CHECK_INDEX(i, count);       return bufferPtr[i]; } 
        [[nodiscard]] const T& operator[](const C i) const  { CHECK_INDEX(i, count);       return bufferPtr[i]; }
        [[nodiscard]] T&       Last()                       { CHECK_INDEX(count-1, count); return bufferPtr[count - 1]; }
        [[nodiscard]] const T& Last()  const                { CHECK_INDEX(count-1, count); return bufferPtr[count - 1]; }
        [[nodiscard]] C Count() const                       { return count; }
        [[nodiscard]] bool     Empty() const                { return count == 0; }

        void Clear() { while (count > 0) bufferPtr[--count].~T(); }

        template<class... Args> 
        T& Append(Args&&... args)
        {
            CHECK_CAPACITY(count+1, CAPACITY);
            return *(new(&bufferPtr[count++]) T{ std::forward<Args>(args)... });
        }

        void Remove(const C i) //O(1)
        {   
            CHECK_INDEX(i, count);
            bufferPtr[i].~T();
            if (--count != i) { 
                new(&bufferPtr[i]) T{ std::move(bufferPtr[count]) }; 
                bufferPtr[count].~T();
            }
        } 

        void RemoveOrdered(C i) //O(n)
        {
            CHECK_INDEX(i, count);
            bufferPtr[i].~T();
            for (; i < count - 1; ++i) {
                bufferPtr[i] = std::move(bufferPtr[i + 1]);
            }
            bufferPtr[--count].~T();
        }

        template<class T2> //allows for custom operator==
        auto Contains(const T2& element) -> T*
        {
            for (C i = 0; i < count; ++i) {
                if (bufferPtr[i] == element) return &bufferPtr[i];
            }
            return nullptr;
        }

        inline void Swap(T& lhs, T& rhs)
        {
            const auto tmp = std::move(lhs);
            lhs = std::move(rhs);
            rhs = std::move(tmp);
        }

        void Reverse()
        {
            for (C i = 0; i < (C)(count * 0.5f); ++i) {
                Swap(bufferPtr[i], bufferPtr[count - 1 - i]);
            }
        }
        
        //auto FindInsertionPoint(const T& toInsert, std::strong_ordering(*fn)(const T& lhs, const T& rhs))
        //{   
        //    auto L = 0;
        //    auto R = count - 1;
        //    int m = 0;
        //
        //    while (L <= R) {
        //        m = (L + R) / 2;
        //        if (fn(bufferPtr[m], toInsert) == std::strong_ordering::less) {
        //            L = m + 1;
        //        }
        //        else if (fn(bufferPtr[m], toInsert) == std::strong_ordering::greater) {
        //            R = m - 1;
        //        }
        //        else {
        //            return m;
        //        }
        //    }
        //    return m;
        //}

        template<class... Args>
        T& Insert(const C insertIdx, Args&&... args)
        {
            if (count == 0)
                return Append(std::forward<Args>(args)...);

            new(&bufferPtr[count]) T(std::move(bufferPtr[count - 1]));
            for (C i = 1; i < (count - insertIdx); ++i) {
                bufferPtr[count - i] = std::move(bufferPtr[count - i - 1]);
            }
            ++count;
            return bufferPtr[insertIdx] = { args... };
            //return bufferPtr[insertIdx];
        }

        void Sort(bool(*fn)(const T& lhs, const T& rhs))
        {
            for (C i = 0; i < count; ++i) { 
                if (fn(bufferPtr[i], bufferPtr[i + 1])) {
                    Swap(bufferPtr[i], bufferPtr[i + 1]);
                }
            }
        }

    protected:
        T* const bufferPtr;
        C  count;
    public:
        const C CAPACITY;
        using DATA_TYPE  = T;
        using COUNT_TYPE = C;
    };


    template<class T, std::size_t CAPACITY_T, typename C = u16>
    class Array final : public IArray<T, C>
    {
    public:
        static constexpr auto SIZE = CAPACITY_T * sizeof(T);

        Array() : IArray<T, C> { reinterpret_cast<T* const>(buffer), CAPACITY_T } { ; }

        template<class... Args, typename = std::enable_if_t<std::is_same_v<T, std::common_type_t<Args...>>>>
        Array(Args&&... elements) : Array() //no forwarding ctor values but T elements (cause tmp creation)
        { 
            ((new(&buffer[this->count++ * sizeof(T)]) T{ std::forward<Args>(elements) }), ...);
        }

        Array(const Array& other) : Array() { this->operator=<Array>(other); }
        Array(Array&& other) : Array() { this->operator=<Array>(std::move(other)); }

        //can be used for resize
        ANY_SIZE Array(const A& other) : Array() { this->operator=<A>(other); }
        ANY_SIZE void operator=(const A& other)
        {
            this->count = (CAPACITY_T >= other.Count()) ? other.Count() : CAPACITY_T;
            for (C i = 0; i < this->count; ++i) ctor(i, other[i]);
        }

        ANY_SIZE Array(A&& other) : Array() { this->operator=<A>(std::move(other)); }
        ANY_SIZE void operator=(A&& other)
        {
            this->count = (CAPACITY_T >= other.Count()) ? other.Count() : CAPACITY_T;
            for (C i = 0; i < this->count; ++i) ctor(i, std::move(other[i]));
        }

    private:
        inline void ctor(const std::size_t& i, T&& t)       { new(&buffer[i * sizeof(T)]) T{ std::move(t) }; }
        inline void ctor(const std::size_t& i, const T& t)  { new(&buffer[i * sizeof(T)]) T{ t }; }

        alignas(T) std::byte buffer[sizeof(T[CAPACITY_T])]; //avoid ctor calls
   };

}//ns

#undef ANY_SIZE
#undef CHECK_INDEX
#undef CHECK_CAPACITY