//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"
#include "com/utils/Utils.hpp"
#include "dbg/Logger.hpp"
#include "dbg/Assert.hpp"
#include "com/box/String.hpp"
#include "com/box/EnumMap.hpp"
#include "com/Range.hpp"

namespace rpg::com {

//TODO: same keys are not considered yet
//this is not ordered! but bucket order based
#define FOR_STRING_MAP_BEGIN_CONST(map, item) \
for(idx_t bucketIdx = 0; bucketIdx < map.BUCKET_COUNT; ++bucketIdx){ \
    const auto& bucket = map.buckets[bucketIdx]; \
    for(idx_t contentIdx = 0; contentIdx < bucket.count; ++contentIdx){ \
        const auto& item = bucket.content[contentIdx];
#define FOR_STRING_MAP_END }}


template<typename VAL, auto STRING_CAPACITY>
struct StringMap
{
    using KEY_T = com::String<STRING_CAPACITY>;
    using VAL_T = VAL;

    static constexpr auto BUCKET_COUNT    = 20;
    static constexpr auto BUCKET_CAPACITY = 20;

    //? STRUCTS

    struct Pair
    {
        KEY_T key;
        VAL_T val;
    };

    struct Bucket
    {
        idx_t count = 0;
        Pair  content [BUCKET_CAPACITY];

        void Add(const Pair& pair) {
            content[count] = pair;
            ++count;
        }
    };

    //? DATA AND CTOR

    Bucket buckets [BUCKET_COUNT];

    StringMap() = default;
    StringMap(std::initializer_list<Pair> pairs)
    {
        for(const auto& pair : pairs) {
            Insert(pair);
        }
    }

    void Insert(const Pair& pair)
    {
        const auto hash = SimpleHash(pair.key.Data());
        buckets[hash].Add(pair);
    }

    //? GET

    const VAL* GetOptional(chars_t key) const
    {
        const auto hash = SimpleHash(key);
        auto& bucket = buckets[hash];
        
        if (bucket.count == 0)
            return nullptr;

        FOR_C_ARRAY(bucket.content, i){
            auto& pair = bucket.content[i];
            if (pair.key == key) //str operator==
                return &pair.val;
        }
        return nullptr;
    }

    const VAL& Get(chars_t key) const
    {
        const auto hash = SimpleHash(key);
        auto& bucket = buckets[hash];
        
        if (bucket.count == 0)
            dbg::Assert(false, "cannot get from string map");

        FOR_C_ARRAY(bucket.content, i){
            auto& pair = bucket.content[i];
            if (pair.key == key) //str operator==
                return pair.val;
        }
        dbg::Assert(false, "cannot get from string map");
        return bucket.content[0].val; //dangling but there is an assert
    }

    //? INTERNAL

    idx_t SimpleHash(chars_t str) const
    {
        const auto c1 = str[0];
        const auto c2 = str[com::StrLen(str) - 1];
        return (c1 + c2) % BUCKET_COUNT;
    }

    idx_t SimpleHash(const com::CharsView& view) const
    {
        dbg::Assert(view.len > 0, "chars view count is not greater than 0");
        const auto c1 = *(view.beginPtr);
        const auto c2 = *(view.beginPtr + view.len - 1);
        return (c1 + c2) % BUCKET_COUNT;
    }

};

template<typename VAL, auto STRING_CAPACITY>
void PrintStringMap(const StringMap<VAL, STRING_CAPACITY>& map)
{
    constexpr auto W = 20;

    std::cout << std::left 
        << std::setw(W) << "BUCKET"
        << std::setw(W) << "BUCKET IDX"
        << std::setw(W) << "KEY"
        << std::setw(W) << "VALUE"
        << '\n';

    FOR_STRING_MAP_BEGIN_CONST(map, item)
    std::cout << std::left 
        << std::setw(W) << bucketIdx
        << std::setw(W) << contentIdx
        << std::setw(W) << item.key
        << std::setw(W) << item.val
        << '\n';
    FOR_STRING_MAP_END
}

template<typename VAL, auto STRING_CAPACITY, auto ENUM_END_T>
auto StringMapFromEnumMap(const com::EnumMap<ENUM_END_T, com::String<STRING_CAPACITY>>& enumMap)
{
    StringMap<VAL, STRING_CAPACITY> strMap;
    for(idx_t i = 0; i < enumMap.ENUM_END; ++i) {
        dbg::Assert(enumMap.Contains(i), "enum missing");
        strMap.Insert({enumMap.Get(i), (VAL)i});
    }
    return strMap;
}

} //NS