#pragma once
#include "MiniSTL/Types.hpp"
#include "MiniSTL/Debug/Assert.hpp"

namespace mini::box
{
#define DO_BOUNDS_CHECK 1
#define FOR_ARRAY(arr, i) for(decltype(arr.Count()) i = 0; i < arr.Count(); ++i)

#define DC [[nodiscard]]

    template<class T, typename CT> 
    class IArray //array size agnostic and type aware (used for passing)
    {
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
        const COUNT_T COUNT_MAX;

        void Clear() { while (count > 0) dataPtr[--count].~T(); }
        
        ///ACCESSORS

        template<typename IDX, typename = IsIntegralOrEnum<IDX>> DC
        T& operator[](const IDX i) 
        { 
            CheckBounds(i, count);
            return dataPtr[(CT)i];
        }

        template<typename IDX, typename = IsIntegralOrEnum<IDX>> DC
        const T& operator[](const IDX i) const
        { 
            CheckBounds(i, count);
            return dataPtr[(CT)i];
        }

        DC T&       Last()        { CheckBounds(count - 1, count); return dataPtr[count - 1]; }
        DC const T& Last()  const { CheckBounds(count - 1, count); return dataPtr[count - 1]; }
        DC CT       Count() const { return count; }
        DC bool     Empty() const { return count == 0; }

        ///APPEND

        template<class... CtorArgs>
        T& Append(CtorArgs&&... args) 
        {
            CheckBounds(count, COUNT_MAX);
            return *(new(&dataPtr[count++]) T{ std::forward<CtorArgs>(args)... });
        }

        ///REMOVE

        template<typename IDX, typename = IsIntegralOrEnum<IDX>>
        void RemoveOrdered(const IDX i) //O(n)
        {
            CheckBounds(i, count);

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
            CheckBounds(i, count);

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
            CheckBounds(insertIdx, count);

            //move elements to the right
            new(&dataPtr[count]) T(std::move(dataPtr[count - 1])); //out of bounds, needs "new"
            for (CT i = count - 1; i >= count - 1 - (CT)insertIdx; --i) { //cast dangerous
                dataPtr[i] = std::move(dataPtr[i-1]);
            }

            ++count;
            return dataPtr[(CT)insertIdx] = { args... };
        }

        ///CHECK

    protected:
        template<typename IDX>
        inline bool CheckBounds(const IDX i, const CT size) const 
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


    template<class T, auto COUNT_MAX_T, typename = IsArraySize<COUNT_MAX_T>>
    class Array final : public IArray<T, IntegralTypeEnum<COUNT_MAX_T>>
    {
    public:
        using COUNT_T = IntegralTypeEnum<COUNT_MAX_T>;
        using DATA_T  = T;

        static constexpr auto COUNT_MAX  = (COUNT_T)COUNT_MAX_T;
        static constexpr auto BYTE_COUNT = sizeof(T) * COUNT_MAX;

        ///CTOR

        constexpr Array() : IArray<T, COUNT_T> { reinterpret_cast<T*>(data), 0, COUNT_MAX }
        { ; }

        template<class... Elements, typename = ParamTypeMatchesT<T, Elements...>>
        Array(Elements&&... elements) : Array()
        {
            //unfold
            ((new(&data[this->count++ * sizeof(T)]) T { std::forward<Elements>(elements) }), ...);
        }

        ///COPY / MOVE CTOR

        //same type and same size
        Array(const Array& other) : Array() { this->operator=<COUNT_T>(other); }
        Array(Array&& other) : Array()      { this->operator=<COUNT_T>(std::move(other)); }
        
        //same type and other size
        template<typename C> Array(const IArray<T, C>& other) : Array() { this->operator=<C>(other); }
        template<typename C> Array(IArray<T, C>&& other) : Array()      { this->operator=<C>(std::move(other)); }

        ///COPY / MOVE ASSIGNMENT
        
        template<typename C> 
        Array& operator=(const IArray<T, C>& other)
        {
            this->count = (COUNT_MAX > other.Count()) ? other.Count() : COUNT_MAX; //clamp
            FOR_ARRAY((*this), i)
            {
                new(&data[i * sizeof(T)]) T { other[i] };
            }
            return *this;
        }

        template<typename C> 
        Array& operator=(IArray<T, C>&& other)
        {
            this->count = (COUNT_MAX > other.Count()) ? other.Count() : COUNT_MAX; //clamp
            FOR_ARRAY((*this), i)
            {
                new(&data[i * sizeof(T)]) T{ std::move(other[i]) };
            }
            return *this;
        }

    private:
        alignas(T) u8 data[BYTE_COUNT];
    };

#undef DC
#undef DO_BOUNDS_CHECK
#undef ANY_SIZE

}//ns

/*
template<class Arr>
Arr InitCompleteArray()
{
    Arr arr;
    for(auto i=0; i< arr.COUNT_MAX; ++i) //could be done more efficient?
        arr.Append();
    return arr;
}
*/

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