//https://github.com/awwdev

/* 
## mini::box::Array

- fixed size (capacity-based with internal count)
    - auto growth can be harmful on perf (reallocations), can lead to ptr invalidation and makes it harder to use with custom allocators
    - simple inheritance (without vtable) is used so the (abstract) base can be passed around without the need of writing Array<T, N> everywhere
    - wrapper and elements are not seperated in memory and the whole object can simply be passed to an allocator
    - AppendArray() can be used to resize or convert an array 
- no ctor calls when initializing the array (underlying byte array)
- array is usable with enums (no cast by the user needed)
- bounds checking is toggleable via macro, no exceptions are used
- removal of elements can be done fast (swapping) or slow while preserving order
- avoidance of cpy/mv ctor and assignment boilerplate - instead use generic AppendArray
- overall more readable and extendible than the STL
*/

#pragma once
#include "MiniSTL/Types.hpp"
#include "MiniSTL/Debug/Assert.hpp"
#include "MiniSTL/Debug/Logger.hpp"

namespace mini::box
{
#define DO_BOUNDS_CHECK 1
#define FOR_ARRAY(arr, i) for(decltype(arr.Count()) i = 0; i < arr.Count(); ++i)
#define ND [[nodiscard]]

    
    //size agnostic in type - use to pass array around like IArray<T>&
    template<class T, typename IDX_T = u32>
    struct IArray
    {
        using DATA_T  = T;
        using INDEX_T = IDX_T; 

        const IDX_T         COUNT_MAX;
        const std::size_t   BYTE_SIZE;


        ///ACCESS

        template<typename IDX, typename = IsIntegralOrEnum<IDX>> ND
        T& operator[](const IDX i) 
        { 
            CheckBounds(i, count);
            return dataPtr[static_cast<IDX_T>(i)];
        }

        template<typename IDX, typename = IsIntegralOrEnum<IDX>> ND
        const T& operator[](const IDX i) const
        { 
            CheckBounds(i, count);
            return dataPtr[static_cast<IDX_T>(i)];
        }


        ND T&       Last()        { CheckBounds(count - 1, count); return dataPtr[count - 1]; }
        ND const T& Last()  const { CheckBounds(count - 1, count); return dataPtr[count - 1]; }
        ND IDX_T    Count() const { return count; }
        ND bool     Empty() const { return count == 0; }

        void Clear() { while (count > 0) dataPtr[--count].~T(); }


        ///ELEMENT OPERATIONS

        template<class... CtorArgs>
        void Append(CtorArgs&&... args)
        {
            CheckBounds(count, COUNT_MAX + 1);
            PlacementNew(count++, std::forward<CtorArgs>(args)...);
        }


        template<class... CtorArgs, typename IDX, typename = IsIntegralOrEnum<IDX>>
        void Insert(const IDX pos, CtorArgs&&... args)
        {
            CheckBounds(pos, count);

            //move elements to the right (copy below into ahead)
            new(&dataPtr[count]) T(std::move(dataPtr[count - 1]));
            for (IDX_T i = count - 1; i > static_cast<IDX_T>(pos); --i) {
                dataPtr[i] = std::move(dataPtr[i - 1]);
            }
            PlacementNew(pos, std::forward<CtorArgs>(args)...); //ctor only is sufficient too

            ++count;
        }


        //does preserve order - O(n)
        template<typename IDX, typename = IsIntegralOrEnum<IDX>>
        void RemoveStable(const IDX pos) 
        {
            CheckBounds(pos, count);

            if constexpr (std::is_trivially_copyable_v<T>)
            {
                std::memmove(&dataPtr[static_cast<IDX_T>(pos)], &dataPtr[static_cast<IDX_T>(pos) + 1], sizeof(T) * (count - 1 - pos));
            }
            else
            {
                dataPtr[static_cast<IDX_T>(pos)].~T();
                for (IDX_T j = static_cast<IDX_T>(pos); j < count - 1; ++j) { //shift to left
                    dataPtr[j] = std::move(dataPtr[j + 1]);
                }
                dataPtr[count].~T();
            }

            --count;
        }

        //does not preserve order O(1)
        template<typename IDX, typename = IsIntegralOrEnum<IDX>>
        void RemoveFast(const IDX pos) 
        {
            CheckBounds(pos, count);
            dataPtr[static_cast<IDX_T>(pos)] = std::move(dataPtr[count - 1]);
            dataPtr[count - 1].~T();
            --count;
        }

        
        ///ARRAY OPERATION

        template<class... CtorArgs>
        void SetCompleteArray(CtorArgs&&... args)
        {
            for (; count < COUNT_MAX; ++count) {
                PlacementNew(count, std::forward<CtorArgs>(args)...);
            }
        }


        void Sort(bool(*fn)(const T& lhs, const T& rhs))
        {
            for (IDX_T i = 0; i < count; ++i) {
                if (fn(dataPtr[i], dataPtr[i + 1])) {
                    Swap(dataPtr[i], dataPtr[i + 1]);
                }
            }
        }


        void Reverse()
        {
            for (IDX_T i = 0; i < (IDX_T)(count * 0.5f); ++i) {
                Swap(dataPtr[i], dataPtr[count - 1 - i]);
            }
        }


        template<class T2> ND
        T* Contains(const T2& element) //allows for custom operator==
        {
            FOR_ARRAY((*this), i)
            {
                if (dataPtr[i] == element)
                    return &dataPtr[i];
            }
            return nullptr;
        }


        ///ARRAY FUSION

        template<class T2, typename IDX_T_>
        void AppendArray(const IArray<T2, IDX_T_>& other)
        {
            CheckBounds(count + other.Count(), COUNT_MAX + 1);
            FOR_ARRAY(other, i) {
                PlacementNew(count++, other[i]);
            }
        }


        template<class T2, typename IDX, typename = IsIntegralOrEnum<IDX>>
        void InsertArray(const IDX pos, const IArray<T2>& other)
        {
            CheckBounds(count + other.Count(), COUNT_MAX + 1);
            static_assert(false, "Not yet implemented.");
            FOR_ARRAY(other, i) {
                dataPtr[pos + i] = other[i];
            }
        }


    protected:
        T* const dataPtr;
        IDX_T    count;

        IArray(T* const data, const IDX_T pCount, const IDX_T countMax, const std::size_t byteSize)
            : dataPtr   { data }
            , count     { pCount }
            , COUNT_MAX { countMax }
            , BYTE_SIZE { byteSize }
        {;}

        IArray(const IArray&)            = delete; //avoid boilerplate
        IArray& operator=(const IArray&) = delete;

        ~IArray() { Clear(); }


        ///INTERNAL

        template<class... CtorArgs>
        inline void PlacementNew(const IDX_T idx, CtorArgs&&... args)
        {
            if constexpr (std::is_aggregate_v<T>)
            {
                new(&dataPtr[idx]) T{ std::forward<CtorArgs>(args)... };
            }
            else
            {
                new(&dataPtr[idx]) T(args...); //POD (only on single param)(will also implicitly cast)   
            }
        }

        template<typename IDX>
        void CheckBounds(const IDX i, const IDX_T size) const
        {
        #if (DO_BOUNDS_CHECK)
            if constexpr (std::is_enum_v<IDX>)
            {
                using UT = std::underlying_type_t<IDX>;
                if ((UT)i < 0 || (UT)i >= size)
                {
                    mini::dbg::dlog<mini::dbg::ColorMode::Red>("Array access out of bounds");
                    __debugbreak();
                }
            }
            else
            {
                if (i < 0 || i >= size)
                {
                    mini::dbg::dlog<mini::dbg::ColorMode::Red>("Array access out of bounds");
                    __debugbreak();
                }
            }
        #endif
        }

        inline void Swap(T& lhs, T& rhs)
        {
            const auto tmp = std::move(lhs);
            lhs = std::move(rhs);
            rhs = std::move(tmp);
        }
    };


    template<class T, auto COUNT_MAX_T, typename IDX_T = u32, typename = IsArraySize<COUNT_MAX_T>>
    struct Array : IArray<T, IDX_T>
    {
        using BASE    = IArray<T, IDX_T>;
        using DATA_T  = T;
        using INDEX_T = IDX_T;

        static constexpr IDX_T COUNT_MAX = (IDX_T) COUNT_MAX_T;
        static constexpr std::size_t BYTE_SIZE = sizeof(T[COUNT_MAX]);

        ///CTOR

        Array() : BASE(reinterpret_cast<T*>(data), 0, COUNT_MAX, BYTE_SIZE)
        {;}

        template<class... ELEMENTS, typename = std::common_type_t<T, ELEMENTS...>>
        explicit Array(const ELEMENTS&... elements) : Array()
        {
            ((new(&data[this->count++ * sizeof(T)]) T(elements)), ...);
        }

    private:
        alignas(T) u8 data[BYTE_SIZE];
    };


    ///OTHER

    template<class ARR, typename = IsPrintable<typename ARR::DATA_T>>
    void PrintArray(const ARR& arr, const char* name = "")
    {
        mini::dbg::dlog(name);
        FOR_ARRAY(arr, i) {
            mini::dbg::dlog(arr[i]);
        }
        mini::dbg::dlog("---");
    }


    template<class... ARRAYS>
    auto CombineArrays(const ARRAYS&... arrays)
    {
        //calc needed size
        //get common type
        //append unfolded
        static_assert(false, "Not yet implemented.");
    }

} //ns

#undef ND
#undef DO_BOUNDS_CHECK