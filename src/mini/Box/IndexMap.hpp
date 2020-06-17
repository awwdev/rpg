//https://github.com/awwdev

/* 
## mini::box::Map2

- index based    (no bucket hash system) usage of indicies and enums
- capacity-based (no dynamic allocations)
- bounds checking is toggleable via macro, no exceptions are used

*/

//? currently there is box::Map which uses a byte array while IndexMap uses a T array 
//? names are wip and are not correct this way

#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Debug/Assert.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Box/Bitset.hpp"

namespace mini::box
{
    #define FOR_INDEX_MAP_BEGIN(map, i) for(auto i = 0; i < map.CAPACITY; ++i) { \
    if (map.Contains(i) == false) continue; 
    #define FOR_INDEX_MAP_END }


    template<class KEY, class VAL, typename = IsIntegralOrEnum<KEY>>
    struct IndexMapPair
    {
        KEY key;
        VAL val;
    };

    //use the base to pass the object around without the need of defining the size in the type
    template<class VAL>
    struct BaseIndexMap
    {
        using Val_t   = VAL;
        using Index_t = u32;
        const Index_t CAPACITY;

        #define KEY_T template<typename KEY, typename = IsIntegralOrEnum<KEY>>

        //? SETTERS

        template<typename KEY, class... CtorArgs, typename = IsIntegralOrEnum<KEY>>
        void Set(const KEY key, CtorArgs&&... args)
        {
            bitsetPtr->Set<true>(key);
            valuesPtr[(Index_t)key] = VAL{ std::forward<CtorArgs>(args)... };
        }

        //? GETTERS

        KEY_T VAL&       Get(const KEY key)                 { return valuesPtr[(Index_t)key]; }
        KEY_T const VAL& Get(const KEY key) const           { return valuesPtr[(Index_t)key]; }
        KEY_T VAL*       GetOptional(const KEY key)         { return Contains(key) ? &valuesPtr[(Index_t)key] : nullptr; }
        KEY_T const VAL* GetOptional(const KEY key) const   { return Contains(key) ? &valuesPtr[(Index_t)key] : nullptr; }

        //? MOD

        KEY_T void Remove(const KEY key) { bitsetPtr->Set<false>(key); }  
        KEY_T bool Contains(const KEY key) const { return bitsetPtr->Test(key); }

        void Clear() { bitsetPtr->Clear(); }

        #undef KEY_T

    protected:
        BaseIndexMap(VAL* const values, IBitset<Index_t>* const bitset, const Index_t capacity) 
            : valuesPtr { values }
            , bitsetPtr { bitset }
            , CAPACITY  { capacity } 
        {;}

        VAL* const valuesPtr;
        IBitset<u32>* const bitsetPtr;

        ~BaseIndexMap() { Clear(); }
    };


    template<class VAL, auto CAPACITY_T, typename = IsArraySize<CAPACITY_T>>
    struct IndexMap : BaseIndexMap<VAL>
    {
        using Index_t = u32;
        using Val_t   = VAL;
        using Base_t  = BaseIndexMap<VAL>;
        using Pair_t  = IndexMapPair<decltype(CAPACITY_T), VAL>;

        constexpr static auto CAPACITY  = (Index_t)CAPACITY_T;
        constexpr static auto BYTE_SIZE = (Index_t)CAPACITY_T * sizeof(VAL);

        //? CTOR

        IndexMap()
            : Base_t(values, &bitset, CAPACITY) 
            , values {} //initList
            , bitset {}
        {;}

        //ctor allow for out of order input
        //use first param with int to invoke variadic ctor (that does not use init list)
        template <class... Elements, typename = DoesTypeMatchParams<Pair_t, Elements...>>
        IndexMap(int, Elements... elements) : IndexMap()
        {
            const auto fn = [&](const Pair_t& pair){
                this->Set(pair.key, pair.val);
                bitset.Set<true>(pair.key);
            };
            (fn(elements), ...);
        }

        IndexMap(std::initializer_list<Pair_t> initList)
            : Base_t(values, &bitset, CAPACITY) 
            , values {}
            , bitset {}
        {
            for(auto& l : initList)
                this->Set(l.key, l.val);
        }

        //? DATA

    private:
        VAL values [(Index_t)CAPACITY_T];
        Bitset<CAPACITY_T> bitset;
    };

}//ns