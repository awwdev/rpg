//https://github.com/awwdev

/* 
## mini::box::Map

- index based (enum can be used, no cast needed)(no bucket hash system)
- fixed size (capacity-based with internal count)(no dynamic allocations)
- no ctor calls when initializing the map (underlying byte array)
- bounds checking is toggleable via macro, no exceptions are used
- overall more readable and extendible than the STL

*/

#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Debug/Assert.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Box/Bitset.hpp"

//TODO: bound checks

namespace mini::box
{
    template<class KEY, class VAL, typename = IsIntegralOrEnum<KEY>>
    struct Pair
    {
        KEY key;
        VAL val;
    };


    template<class VAL>
    struct INewMap
    {
        using Val_t = VAL;
        const std::size_t CAPACITY;

        //? METHODS

        template<typename KEY, typename = IsIntegralOrEnum<KEY>>
        VAL& operator[](const KEY key)
        {
            //todo: check bounds

            //if (bitsetPtr->Test(key))
            //    dataPtr[key].~T();

            //bitset.Set<true>(key);

            return dataPtr[key];
        }

        //! QUESTION: DO INIT COMPLETE MAP ?? 
        //or init when needed (more checks ...)
        //maybe let user decide (template<DoInit::YES>)

        template<typename IDX, typename = IsIntegralOrEnum<IDX>>
        bool Contains(const IDX i)
        {
            return bitsetPtr->Test(i);
        }

        /*
        template<typename IDX, class... CtorArgs, typename = IsIntegralOrEnum<IDX>>
        void Set(const IDX idx, CtorArgs&&... args)
        {
            if (bitset.Test(idx))
                dataPtr[idx].~T();

            bitset.Set<true>(idx);
            
            if constexpr (std::is_aggregate_v<T>)
            {
                new(&dataPtr[idx]) T{ std::forward<CtorArgs>(args)... };
            }
            else
            {
                new(&dataPtr[idx]) T(args...);
            }
        }

        template<typename IDX, class... CtorArgs, typename = IsIntegralOrEnum<IDX>>
        T& Get(const IDX idx)
        {
            return dataPtr[idx];
        }

        template<typename IDX, class... CtorArgs, typename = IsIntegralOrEnum<IDX>>
        T* GetOptional(const IDX idx)
        {
            return Contains(idx) ? &dataPtr[idx] : nullptr;
        }

        void Remove()
        {
            //TODO
            static_assert(false, "no impl yet");
        }
        */

        void Clear()
        {
            //for(u32 i = 0; i < CAPACITY; ++i)
            //{
            //    if (bitsetPtr->Test(i))
            //        dataPtr[i].~T();
            //}
            //bitset->Clear();
        }

    protected:
        INewMap(VAL* const data, IBitset<u32>* const bitset, const u32 capacity) 
            : dataPtr   { data }
            , bitsetPtr { bitset }
            , CAPACITY  { capacity } 
        {;}

        VAL* const dataPtr;
        IBitset<u32>* const bitsetPtr;

        ~INewMap() { Clear(); }
    };


    template<class VAL, auto CAPACITY_T, typename = IsArraySize<CAPACITY_T>>
    struct NewMap : INewMap<VAL>
    {
        constexpr static auto CAPACITY  = CAPACITY_T;
        constexpr static auto BYTE_SIZE = CAPACITY * sizeof(VAL);

        using Val_t  = VAL;
        using Base_t = INewMap<VAL>;
        using Pair_t = Pair<decltype(CAPACITY), VAL>;

        NewMap() : Base_t(reinterpret_cast<VAL*>(data), &bitset, CAPACITY) {;}

        template <class... Elements, typename = DoesTypeMatchParams<Pair_t, Elements...>>
        NewMap(Elements... elements) : NewMap()
        {

        }

        private:
            alignas(VAL) u8 data[BYTE_SIZE];
            Bitset<CAPACITY_T> bitset;
    };








    //for iteration keep capacity small
    #define FOR_MAP_BEGIN(map, i) for(auto i = 0; i < map.CAPACITY; ++i) { \
    if (map.Contains(i) == false) continue; 
    #define FOR_MAP_END }

    template<class T>
    struct Exist { bool doesExist; T obj; };

    //use for passing around
    template<class T>
    struct IMap
    {
        template<typename IDX, typename = IsIntegralOrEnum<IDX>>
        bool Contains(const IDX i)
        {
            return bitset.Test(i);
        }

        template<typename IDX, class... CtorArgs, typename = IsIntegralOrEnum<IDX>>
        void Set(const IDX idx, CtorArgs&&... args)
        {
            if (bitset.Test(idx))
                dataPtr[idx].~T();

            bitset.Set<true>(idx);
            
            if constexpr (std::is_aggregate_v<T>)
            {
                new(&dataPtr[idx]) T{ std::forward<CtorArgs>(args)... };
            }
            else
            {
                new(&dataPtr[idx]) T(args...);
            }
        }

        template<typename IDX, class... CtorArgs, typename = IsIntegralOrEnum<IDX>>
        T& Get(const IDX idx)
        {
            return dataPtr[idx];
        }

        template<typename IDX, class... CtorArgs, typename = IsIntegralOrEnum<IDX>>
        T* GetOptional(const IDX idx)
        {
            return Contains(idx) ? &dataPtr[idx] : nullptr;
        }

        void Remove()
        {
            //TODO
            static_assert(false, "no impl yet");
        }

        void Clear()
        {
            for(u32 i = 0; i < CAPACITY; ++i)
            {
                if (bitset.Test(i))
                    dataPtr[i].~T();
            }
            bitset.Clear();
        }

        const u32 CAPACITY;

    protected:
        IMap(T* const data, const u32 capacity) : dataPtr { data }, CAPACITY { capacity } {;}
        T* const dataPtr;
        box::Bitset<100> bitset; //!issue !!! 
        //need of IBitset too (size agnostic)

        ~IMap() { Clear(); }
    };

    template<class T, auto CAPACITY_T, typename = IsArrayIndex<CAPACITY_T>>
    struct Map : IMap<T>
    {
        using BASE = IMap<T>;

        constexpr static auto CAPACITY  = CAPACITY_T;
        constexpr static auto BYTE_SIZE = CAPACITY * sizeof(T);

        Map() : BASE(reinterpret_cast<T*>(data), CAPACITY) {;}

    private:
        alignas(T) u8 data[BYTE_SIZE];
    };

}//ns