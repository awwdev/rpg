//https://github.com/awwdev

#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Box/Bitset.hpp"
#include "mini/Box/Array.hpp"

namespace mini::box {

    
template<class KEY, class VAL, typename = IsIntegralOrEnum<KEY>>
struct EnumPair
{
    KEY key;
    VAL val;
};

template<auto ENUM_END, class VAL>
struct EnumMap
{
    //? DATA

    using PAIR = EnumPair<decltype(ENUM_END), VAL>;
    using KEY  = decltype(ENUM_END);

    VAL data [ENUM_END];
    box::Array<idx_t, (idx_t)ENUM_END> usedIndices; //for iteration
    box::Bitset<ENUM_END> bitset;

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
        bitset.Set(key, false);
        FOR_ARRAY(usedIndices, i){
            if(usedIndices[i] == (idx_t)key)
        }
    }

    void Clear() 
    {
        bitset.Clear();
        usedIndices.Clear();
    }

};

}//ns