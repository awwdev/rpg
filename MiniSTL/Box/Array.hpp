#pragma once
#include "MiniSTL/Types.hpp"
#include "MiniSTL/Debug/Assert.hpp"
#include <array>

namespace mini::box
{
    /*
    DESCRIPTION
    - Fixed size (auto growth would be problematic for perf and allocators, also invalidation of ptrs)
    - Arrays can be copied/moved into other sized arrays with compatible type (may result in loss of data)
    - Inheritance (without virtuality) makes passing around the array convenient (no size in type needed)
    - Array can handle enums (no outer cast needed)
    - No exceptions and checks can be toggle via macro
    - Remove is O(1) and will not preserve order (vs RemoveOrdered which is O(n))
    - Initialization of the array will not trigger ctors of objects
    - Simpler to read, debug and probably extend than the STL
    */

#define DO_BOUNDS_CHECK 1
#define FOR_ARRAY(arr, i) for(decltype(arr.Count()) i = 0; i < arr.Count(); ++i)
#define DC [[nodiscard]]

    template<class T, typename CT = u32> 
    class IArray //array size agnostic (used for passing)
    {
    protected: //user shall not create an IArray but Array
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
            Check_PositiveAndLess(i, count);
            return dataPtr[static_cast<CT>(i)];
        }

        template<typename IDX, typename = IsIntegralOrEnum<IDX>> DC
        const T& operator[](const IDX i) const
        { 
            Check_PositiveAndLess(i, count);
            return dataPtr[static_cast<CT>(i)];
        }

        DC T&       Last()        { Check_PositiveAndLess(count - 1, count); return dataPtr[count - 1]; }
        DC const T& Last()  const { Check_PositiveAndLess(count - 1, count); return dataPtr[count - 1]; }
        DC CT       Count() const { return count; }
        DC bool     Empty() const { return count == 0; }

        ///APPEND

        template<class... CtorArgs>
        void Append(CtorArgs&&... args) 
        {
            Check_PositiveAndLess(count, COUNT_MAX + 1); 

            if constexpr (std::is_aggregate_v<T>)
            {
                new(&dataPtr[count++]) T{ std::forward<CtorArgs>(args)... }; //struct init possible
            }
            else 
            {
                new(&dataPtr[count++]) T(args...); //pod (will implicit cast if possible) no unfold needed (single param)
            }
        }

        template<class... CtorArgs> DC
        T& AppendReturn(CtorArgs&&... args)
        {
            Append(args...);
            return Last();
        }

        ///REMOVE

        template<typename IDX, typename = IsIntegralOrEnum<IDX>>
        void RemoveOrdered(const IDX i) //O(n)
        {
            Check_PositiveAndLess(i, count);

            dataPtr[i].~T();
            for (CT j = i; j < count - 1; ++j) { //shift to left
                dataPtr[j] = std::move(dataPtr[j + 1]);
            }
            dataPtr[count--].~T();
        }

        template<typename IDX, typename = IsIntegralOrEnum<IDX>>
        void Remove(const IDX i) //O(1)
        {
            Check_PositiveAndLess(i, count);

            dataPtr[i].~T();
            --count;
            if (count != i) //now move last element in this very spot (if we had not removed the last)
            { 
                new(&dataPtr[i]) T{ std::move(dataPtr[count]) };
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
            Check_PositiveAndLess(insertIdx, count);

            //move elements to the right (copy below into ahead)
            new(&dataPtr[count]) T(std::move(dataPtr[count - 1])); //out of bounds, needs "new"
            for (CT i = count - 1; i >= count - 1 - (CT)insertIdx; --i) { 
                dataPtr[i] = std::move(dataPtr[i-1]);
            }

            ++count;
        }

        template<typename IDX, class... CtorArgs, typename = IsIntegralOrEnum<IDX>> DC
        T& InsertReturn(const IDX insertIdx, CtorArgs&&... args)
        {
            Insert(insertIdx, args...);
            return dataPtr[(CT)insertIdx];
        }

    protected:

        template<typename IDX>
        inline bool Check_PositiveAndLess(const IDX i, const CT size) const 
        {
        #if (DO_BOUNDS_CHECK)
            if constexpr (std::is_enum_v<IDX>)
            {
                using UT = std::underlying_type_t<IDX>;
                if ((UT)i < 0 || (UT)i >= size)
                {
                    ErrLOG("Array access out of bounds");
                    __debugbreak();
                    return false;
                }
            }
            else
            {
                if (i < 0 || i >= size)
                {
                    ErrLOG("Array access out of bounds");
                    __debugbreak();
                    return false;
                }
            }
        #endif

            return true;
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

        constexpr Array() : IArray<T> { reinterpret_cast<T*>(data), 0, COUNT_MAX }
        { ; }

        //this ctor shall take elements and not arrays 
        //SFINAE prevents that the compiler uses this as a first class citizen copy ctor
        template<class... Elements, typename = std::enable_if_t<!std::is_base_of_v<IArray<T>, std::common_type_t<Elements...>>>>
        Array(Elements&&... elements) : Array() 
        {
            ((this->Append(std::forward<Elements>(elements))), ...);
        }

        void InitCompleteArray()
        {
            for (COUNT_T i = 0; i < COUNT_MAX; ++i) {
                this->Append();
            }
        }

        ///COPY / MOVE CTOR (any size)

        Array(const Array& other) : Array()     { CopyMoveArray(other); }
        Array(const IArray<T>& other) : Array() { CopyMoveArray(other); }
        void operator=(const Array& other)      { CopyMoveArray(other); }
        void operator=(const IArray<T>& other)  { CopyMoveArray(other); }

        Array(Array&& other) : Array()          { CopyMoveArray(std::move(other)); }
        Array(IArray<T>&& other) : Array()      { CopyMoveArray(std::move(other)); }
        void operator=(Array&& other)           { CopyMoveArray(std::move(other)); }
        void operator=(IArray<T>&& other)       { CopyMoveArray(std::move(other)); }

        template<class Arr>
        void CopyMoveArray(Arr&& other)
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
        DLOG(name);
        FOR_ARRAY(arr, i) {
            DLOG(arr[i]);
        }
        DLOG("---");
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