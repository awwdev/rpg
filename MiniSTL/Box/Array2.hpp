#pragma once
#include "MiniSTL/Types.hpp"
#include "MiniSTL/Debug/Assert.hpp"

namespace mini::box2
{
#define DC [[nodiscard]]
#define FOR_ARRAY(arr, i)   for(decltype(arr.count) i = 0; i < arr.count; ++i)
#define CHECK_BOUNDS(t, n, s) mini::Assert((t)n >= (t)0 && (t)n < (t)s, "Array access out of bounds");

    template<class T, typename CT> 
    struct IArray //array size agnostic and type aware (used for passing)
    {
        using COUNT_T = CT;

        COUNT_T count;
        const COUNT_T COUNT_MAX;
        T* dataPtr;
        
        ///ACCESSORS

        template<typename IDX, typename = IsIntegralOrEnum<IDX>> DC
        T& operator[](const IDX i) 
        { 
            CHECK_BOUNDS(CT, i, COUNT_MAX);
            return dataPtr[(CT)i];
        }

        template<typename IDX, typename = IsIntegralOrEnum<IDX>> DC
        const T& operator[](const IDX i) const
        { 
            CHECK_BOUNDS(CT, i, COUNT_MAX);
            return dataPtr[(CT)i];
        }

        DC T&       Last()                          { return dataPtr[count - 1]; }
        DC const T& Last()  const                   { return dataPtr[count - 1]; }
        DC bool     Empty() const                   { return count == 0; }

        ///APPEND

        template<class... CtorArgs>
        void Append(CtorArgs&&... args)
        {
            CHECK_BOUNDS(CT, count, COUNT_MAX);
            new(&dataPtr[count]) T { std::forward<CtorArgs>(args)... };
            ++count;
        }

        template<class... CtorArgs> DC
        T& AppendReturn(CtorArgs&&... args) 
        {
            CHECK_BOUNDS(CT, count, COUNT_MAX);
            auto ptr = new(&dataPtr[count]) T{ std::forward<CtorArgs>(args)... };
            ++count;
            return *ptr;
        }

        ///REMOVE

        template<typename IDX, typename = IsIntegralOrEnum<IDX>>
        void RemoveOrdered(const IDX i) //O(n)
        {
            CHECK_BOUNDS(CT, i, COUNT_MAX);
            dataPtr[i].~T();
            for (CT j = i; j < count - 1; ++j) { //shift em all
                dataPtr[j] = std::move(dataPtr[j + 1]);
            }
            dataPtr[count].~T();
            --count;
        }

        template<typename IDX, typename = IsIntegralOrEnum<IDX>>
        void Remove(const IDX i) //O(1)
        {
            CHECK_BOUNDS(CT, i, COUNT_MAX);
            dataPtr[i].~T();
            --count;
            if (count != i) //now construct last element in this very spot (if we had not removed the last)
            { 
                new(&dataPtr[i]) T{ std::move(dataPtr[count]) };
                dataPtr[count].~T();
            }
        }

        ///FIND

        template<class T2> //allows for custom operator==
        T* Contains(const T2& element) 
        {
            FOR_ARRAY((*this), i)
            {
                if (dataPtr[i] == element) 
                    return &dataPtr[i];
            }
            return nullptr;
        }

        ///INSERT

        template<typename IDX, class... CtorArgs, typename = IsIntegralOrEnum<IDX>>
        T& Insert(const IDX insertIdx, CtorArgs&&... args)
        {
            CHECK_BOUNDS(CT, insertIdx, COUNT_MAX);

            //TODO

            //new(&bufferPtr[count]) T(std::move(bufferPtr[count - 1]));
            //
            //for (IDX i = 1; i < (count - insertIdx); ++i) {
            //    bufferPtr[count - i] = std::move(bufferPtr[count - i - 1]);
            //}
            //++count;
            //return bufferPtr[insertIdx] = { args... };
            //return bufferPtr[insertIdx];
        }


    };


    template<class T, auto COUNT_MAX_T, typename = IsArraySize<COUNT_MAX_T>>
    struct Array : IArray<T, IntegralTypeEnum<COUNT_MAX_T>>
    {
        using COUNT_T = IntegralTypeEnum<COUNT_MAX_T>;

        static constexpr auto COUNT_MAX  = (COUNT_T)COUNT_MAX_T;
        static constexpr auto BYTE_COUNT = sizeof(T) * COUNT_MAX;

        u8 data[BYTE_COUNT];

        constexpr Array() : IArray<T, COUNT_T> { 0, COUNT_MAX, reinterpret_cast<T*>(data) } { ; }

    };

#undef DC

}//ns