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

  //for iteration keep capacity small
#define FOR_MAP_BEGIN(map, i) for(auto i = 0; i < map.CAPACITY; ++i) { \
if (map.Contains(i) == false) continue; 
#define FOR_MAP_END }

namespace mini::box
{
    template<class KEY, class VAL, typename = IsIntegralOrEnum<KEY>>
    struct Pair
    {
        KEY key;
        VAL val;
    };


    template<class VAL>
    struct IMap
    {
        using Val_t = VAL;
        const std::size_t CAPACITY;

        //? SETTERS

        template<typename KEY, class... CtorArgs, typename = IsIntegralOrEnum<KEY>>
        void Set(const KEY key, CtorArgs&&... args)
        {
            if (bitsetPtr->Test(key))
                dataPtr[key].~VAL();

           bitsetPtr->Set<true>(key);

           if constexpr (std::is_aggregate_v<VAL>)
           {
               new(&dataPtr[key]) VAL{ std::forward<CtorArgs>(args)... };
           }
           else
           {
               new(&dataPtr[key]) VAL(args...);
           }
        }

        //? GETTERS

        template<typename KEY, typename = IsIntegralOrEnum<KEY>>
        VAL& Get(const KEY key)
        {
            return dataPtr[key];
        }

        template<typename KEY, typename = IsIntegralOrEnum<KEY>>
        const VAL& Get(const KEY key) const
        {
            return dataPtr[key];
        }

        template<typename KEY, typename = IsIntegralOrEnum<KEY>>
        VAL* GetOptional(const KEY key)
        {
            return Contains(key) ? &dataPtr[key] : nullptr;
        }

        template<typename KEY, typename = IsIntegralOrEnum<KEY>>
        const VAL* GetOptional(const KEY key) const
        {
            return Contains(key) ? &dataPtr[key] : nullptr;
        }

        //? OTHER

        template<typename KEY, typename = IsIntegralOrEnum<KEY>>
        bool Contains(const KEY key) const
        {
            return bitsetPtr->Test(key);
        }

        template<typename KEY, typename = IsIntegralOrEnum<KEY>>
        void Remove(const KEY key)
        {
            dataPtr[key].~VAL();
            bitsetPtr->Set<false>(key);
        }  

        void Clear()
        {
            for(u32 i = 0; i < CAPACITY; ++i)
            {
                if (bitsetPtr->Test(i))
                    dataPtr[i].~VAL();
            }
            bitsetPtr->Clear();
        }

    protected:
        IMap(VAL* const data, IBitset<u32>* const bitset, const u32 capacity) 
            : dataPtr   { data }
            , bitsetPtr { bitset }
            , CAPACITY  { capacity } 
        {;}

        VAL* const dataPtr;
        IBitset<u32>* const bitsetPtr;

        ~IMap() { Clear(); }
    };


    template<class VAL, auto CAPACITY_T, typename = IsArraySize<CAPACITY_T>>
    struct Map : IMap<VAL>
    {
        constexpr static auto CAPACITY  = CAPACITY_T;
        constexpr static auto BYTE_SIZE = CAPACITY * sizeof(VAL);

        using Val_t  = VAL;
        using Base_t = IMap<VAL>;
        using Pair_t = Pair<decltype(CAPACITY), VAL>;

        Map() : Base_t(reinterpret_cast<VAL*>(data), &bitset, CAPACITY) {;}

        template <class... Elements, typename = DoesTypeMatchParams<Pair_t, Elements...>>
        Map(Elements... elements) : Map()
        {
            const auto fn = [&](const Pair_t& pair){
                this->Set(pair.key, pair.val);
                bitset.Set<true>(pair.key);
            };
            (fn(elements), ...);
        }

        private:
            alignas(VAL) u8 data[BYTE_SIZE];
            Bitset<CAPACITY_T> bitset;
    };

}//ns