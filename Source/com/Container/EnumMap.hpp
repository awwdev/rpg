//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"
#include "com/Container/Bitset.hpp"
#include "com/Container/Array.hpp"

namespace rpg::com {
    
template<class KEY, class VAL, typename = IsIntegralOrEnum<KEY>>
struct EnumPair
{
    KEY key;
    VAL val;
};

//wrapper over array that uses enum for max count and index
//the wrapper allows for order independent init, removal, add, and is capacity based (last enum item)

#define FOR_ENUM_MAP(map, i) for(idx_t i = 0; i < map.ENUM_END; ++i)

template<auto ENUM_END_T, class VAL>
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

    KEY_T VAL&       Get        (const KEY key)       { return data[(idx_t)key]; }
    KEY_T const VAL& Get        (const KEY key) const { return data[(idx_t)key]; }
    KEY_T VAL*       GetOptional(const KEY key)       { return Contains(key) ? &data[(idx_t)key] : nullptr; }
    KEY_T const VAL* GetOptional(const KEY key) const { return Contains(key) ? &data[(idx_t)key] : nullptr; }

    KEY_T bool Contains(const KEY key) const
    {
        return bitset.Test((idx_t)key);
    }

    #undef KEY_T

    //? ADD / REMOVE

    template<class... ARGS>
    void Set(const KEY key, ARGS&&... args)
    {
        data[(idx_t)key] = VAL { std::forward<ARGS>(args)... };
        
        if (!bitset.Test(key)) {
            bitset.Set(key, true);
            usedIndices.Append((idx_t)key);
        }
    }

    void Remove(const KEY key)
    {
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