//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"
#include "com/box/Bitset.hpp"
#include "com/box/Array.hpp"
#include <concepts>

namespace rpg::com {
    
template<class KEY, class VAL, typename = IsIntegralOrEnum<KEY>>
struct EnumPair
{
    using KEY_T = KEY;
    using VAL_T = VAL;
    KEY key;
    VAL val;
};

template<class T>
concept PairConcept = requires (T) { T::KEY_T; T::VAL_T; };

//wrapper over array that uses enum for max count and index
//the wrapper allows for order independent init, removal, add, and is capacity based (last enum item)

template<auto ENUM_END_T, class VAL, auto OFFSET = 0>
struct EnumMap
{
    //? DATA
    static constexpr auto ENUM_END = (idx_t)ENUM_END_T;

    using KEY   = decltype(ENUM_END_T);
    using PAIR  = EnumPair<decltype(ENUM_END_T), VAL>;

    VAL data [ENUM_END];
    com::Array<idx_t, ENUM_END> usedIndices; //for iteration
    com::Bitset<ENUM_END> bitset;

    EnumMap() = default;
    EnumMap(std::initializer_list<PAIR> list)
    {
        for(auto& l : list)
            Set(l.key, l.val);
    }

    //? ACCESS

    #define KEY_T template<class KEY>

    KEY_T VAL&       Get    (const KEY key)       { return data[(idx_t)key]; }
    KEY_T const VAL& Get    (const KEY key) const { return data[(idx_t)key]; }
    KEY_T VAL*       GetPtr (const KEY key)       { return Contains(key) ? &data[(idx_t)key] : nullptr; }
    KEY_T const VAL* GetPtr (const KEY key) const { return Contains(key) ? &data[(idx_t)key] : nullptr; }

    KEY_T bool Contains(const KEY key) const
    {
        const idx_t keyOffsetted = (idx_t) key - (idx_t) OFFSET;
        return bitset.Test(keyOffsetted);
    }

    #undef KEY_T

    //? ADD / REMOVE

    template<class... ARGS>
    void Set(const KEY key, ARGS&&... args)
    {
        const idx_t keyOffsetted = (idx_t) key - (idx_t) OFFSET;

        data[keyOffsetted] = VAL { std::forward<ARGS>(args)... };
        dbg::Assert(bitset.Test(keyOffsetted) == false, "enum entry already exists");

        bitset.Set(keyOffsetted, true);
        usedIndices.AppendElement(keyOffsetted);
    }

    void Remove(const KEY key)
    {
        const idx_t keyOffsetted = (idx_t) key - (idx_t) OFFSET;

        //bitset.Set(key, false);
        //FOR_ARRAY(usedIndices, i){
        //    if(usedIndices[i] == (idx_t)key)
        //}
        dbg::Assert(false, "no impl yet");
    }

    void Clear() 
    {
        bitset.Clear();
        usedIndices.Clear();
    }

};

}//ns