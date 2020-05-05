#pragma once
#include "MiniSTL/Types.hpp"
#include "MiniSTL/Debug/Assert.hpp"
#include <array>

namespace mini::box
{
    /*
    - fixed size: 
        - auto growth can be harmful on perf, can lead to ptr invalidation bugs and makes it harder to use with allocators
        - array can be copied/moved into other sized arrays with convertible type (may result in loss of data)
        - array can be used the same way on both stack and heap (managed by an allocator)
        - due to inheritance (virtuality not used though) you can use the size agnostic base for passing around (no Array<T, SIZE>& but Array<T>&)
        - objects lives completely on either heap or stack (no jumping around like vector)
    - enums can be passed to the array (no outer cast by the user needed)
    - no exceptions - bound checks can be toggle via macro
    - array does not assume you want to preserve order, there is Remove (O(1)) and RemoveOrdered (O(n))
    - array initialization will not trigger ctors of elements
    - should be more readable, debuggable and extendable than the STL
    - while I tend not to use much of OOP (I like procedural and DOD), in this specific case accessors (hide "count" for example) and inheritance make perfect sense
      (because under no imaginable circumstances you would want that the count is set from outside, so there is only a "Getter", also "abstract" IArray shall not be instantiated on its own)
    */

#define DO_BOUNDS_CHECK 1
#define FOR_ARRAY(arr, i) for(decltype(arr.Count()) i = 0; i < arr.Count(); ++i)
#define DC [[nodiscard]]

    //type size agnostic - used for passing around the array
    template<class T, typename CT = u32> 
    class IArray 
    {
    //user shall not create an IArray ("abstract") but Array
    protected: 
        CT count;
        T* dataPtr;

        IArray(T* const data, const CT pCount, const CT countMax)
            : dataPtr   { data }
            , count     { pCount }
            , COUNT_MAX { countMax }
        {}

        ~IArray() { Clear(); }

    public:
        using COUNT_T = CT;
        using DATA_T  = T;
        const CT COUNT_MAX;

        void Clear() { while (count > 0) dataPtr[--count].~T(); }
        
        ///ACCESSORS

        template<typename IDX, typename = IsIntegralOrEnum<IDX>> DC
        T& operator[](const IDX i) 
        { 
            CheckBounds(i, count);
            return dataPtr[static_cast<CT>(i)];
        }

        template<typename IDX, typename = IsIntegralOrEnum<IDX>> DC
        const T& operator[](const IDX i) const
        { 
            CheckBounds(i, count);
            return dataPtr[static_cast<CT>(i)];
        }

        DC T&       Last()        { CheckBounds(count - 1, count); return dataPtr[count - 1]; }
        DC const T& Last()  const { CheckBounds(count - 1, count); return dataPtr[count - 1]; }
        DC CT       Count() const { return count; }
        DC bool     Empty() const { return count == 0; }

        ///APPEND

        template<class... CtorArgs>
        void Append(CtorArgs&&... args) 
        {
            CheckBounds(count, COUNT_MAX + 1);

            if constexpr (std::is_aggregate_v<T>)
            {
                new(&dataPtr[count++]) T{ std::forward<CtorArgs>(args)... };
            }
            else 
            {
                new(&dataPtr[count++]) T(args...); //POD (will also implicit cast)(only on single param)
            }
        }

        template<class... CtorArgs> DC
        T& AppendReturn(CtorArgs&&... args)
        {
            Append(args...);
            return dataPtr[count - 1];
        }

        ///REMOVE

        template<typename IDX, typename = IsIntegralOrEnum<IDX>>
        void RemoveOrdered(const IDX i) //O(n)
        {
            CheckBounds(i, count);

            if constexpr (std::is_trivially_copyable_v<T>)
            {
                std::memmove(&dataPtr[static_cast<CT>(i)], &dataPtr[static_cast<CT>(i) + 1], sizeof(T) * (count - i)); //FIX
            }
            else
            {
                dataPtr[static_cast<CT>(i)].~T();
                for (CT j = static_cast<CT>(i); j < count - 1; ++j) { //shift to left
                    dataPtr[j] = std::move(dataPtr[j + 1]);
                }
                dataPtr[count].~T();
            }

            --count;
        }

        template<typename IDX, typename = IsIntegralOrEnum<IDX>>
        void Remove(const IDX i) //O(1)
        {
            CheckBounds(i, count);

            dataPtr[static_cast<CT>(i)].~T();
            --count;
            if (count != i) //now move last element in this very spot (if we had not removed the last)
            { 
                new(&dataPtr[static_cast<CT>(i)]) T{ std::move(dataPtr[count]) };
                dataPtr[count].~T();
            }
        }

        ///FIND

        template<class T2> DC //allows for custom operator==
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
        void Insert(const IDX insertIdx, CtorArgs&&... args)
        {
            CheckBounds(insertIdx, count);

            //move elements to the right (copy below into ahead)
            new(&dataPtr[count]) T(std::move(dataPtr[count - 1])); //out of bounds, needs "new"
            for (CT i = count - 1; i >= count - 1 - static_cast<CT>(insertIdx); --i) { 
                dataPtr[i] = std::move(dataPtr[i-1]);
            }

            ++count;
        }

        template<typename IDX, class... CtorArgs, typename = IsIntegralOrEnum<IDX>> DC
        T& InsertReturn(const IDX insertIdx, CtorArgs&&... args)
        {
            Insert(insertIdx, args...);
            return dataPtr[static_cast<CT>(insertIdx)];
        }

    protected:

        template<typename IDX>
        void CheckBounds(const IDX i, const CT size) const 
        {
        #if (DO_BOUNDS_CHECK)
            if constexpr (std::is_enum_v<IDX>)
            {
                using UT = std::underlying_type_t<IDX>;
                if (static_cast<UT>(i) < 0 || static_cast<UT>(i) >= size)
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

    };


    template<class T, auto COUNT_MAX_T, typename CT = u32, typename = IsArraySize<COUNT_MAX_T>>
    class Array final : public IArray<T, CT>
    {
    public:
        using COUNT_T = CT;
        using DATA_T  = T;

        static constexpr auto COUNT_MAX  = (COUNT_T)COUNT_MAX_T;
        static constexpr auto BYTE_COUNT = sizeof(T) * COUNT_MAX;

        ///CTOR
        Array() noexcept : IArray<T> { reinterpret_cast<T*>(data), 0, COUNT_MAX }
        { ; }

        template<class... Elements, typename = std::enable_if_t<!std::is_base_of_v<IArray<T>, std::common_type_t<Elements...>>>>
        Array(Elements&&... elements) : Array() 
        {
            ((this->Append(std::forward<Elements>(elements))), ...); //redundant bound checks
        }

        template<class... CtorArgs>
        void InitCompleteArray(CtorArgs&&... args)
        {
            this->Clear();  

            for (CT i = 0; i <= COUNT_MAX; ++i)
            {
                if constexpr (std::is_aggregate_v<T>)
                {
                    new(&data[this->count++ * sizeof(T)]) T{ std::forward<CtorArgs>(args)... };
                }
                else
                {
                    new(&data[this->count++ * sizeof(T)]) T(args...); //POD (will also implicit cast)(only on single param)
                }
            }
        }

        ///COPY / MOVE CTOR (any size)

        Array(const Array& other) : Array()     { CopyOrMoveArray(other); }
        Array(const IArray<T>& other) : Array() { CopyOrMoveArray(other); }
        void operator=(const Array& other)      { CopyOrMoveArray(other); }
        void operator=(const IArray<T>& other)  { CopyOrMoveArray(other); }

        Array(Array&& other) : Array()          { CopyOrMoveArray(std::move(other)); }
        Array(IArray<T>&& other) : Array()      { CopyOrMoveArray(std::move(other)); }
        void operator=(Array&& other)           { CopyOrMoveArray(std::move(other)); }
        void operator=(IArray<T>&& other)       { CopyOrMoveArray(std::move(other)); }

        template<class Arr>
        void CopyOrMoveArray(Arr&& other)
        {
            this->Clear();
            this->count = (COUNT_MAX > other.Count()) ? other.Count() : COUNT_MAX; //clamp

            if constexpr (std::is_lvalue_reference_v<Arr>) //copy
            {
                FOR_ARRAY((*this), i)
                {
                    new(&data[i * sizeof(T)]) T { other[i] };
                }
            }
            else //move
            {
                FOR_ARRAY((*this), i)
                {
                    new(&data[i * sizeof(T)]) T { std::move(other[i]) };
                }
                other.Clear();
            }
        }

    private:
        alignas(T) u8 data[BYTE_COUNT]; //avoids default ctor calls
    };

    template<class Arr, typename = IsPrintable<typename Arr::DATA_T>>
    void PrintArray(const Arr& arr, const char* name = "")
    {
        mini::dbg::dlog(name);
        FOR_ARRAY(arr, i) {
            mini::dbg::dlog(arr[i]);
        }
        mini::dbg::dlog("---");
    }

#undef DC

}//ns

/*
void Sort(bool(*fn)(const T& lhs, const T& rhs))
{
    for (IDX i = 0; i < count; ++i) {
        if (fn(bufferPtr[i], bufferPtr[i + 1])) {
            Swap(bufferPtr[i], bufferPtr[i + 1]);
        }
    }
}
*/

/*
void Reverse()
{
    for (IDX i = 0; i < (IDX)(count * 0.5f); ++i) {
        Swap(bufferPtr[i], bufferPtr[count - 1 - i]);
    }
}
*/

/*
inline void Swap(T& lhs, T& rhs)
{
    const auto tmp = std::move(lhs);
    lhs = std::move(rhs);
    rhs = std::move(tmp);
}
*/