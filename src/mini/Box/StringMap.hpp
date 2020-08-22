//https://github.com/awwdev

#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Utils/Algorithms.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Box/String.hpp"
#include "mini/Box/EnumMap.hpp"
#include "mini/Utils/CharsView.hpp"

namespace mini::box {

//TODO: same keys are not considered yet

#define FOR_STRING_MAP_BEGIN_CONST(map, item) \
for(idx_t bucketIdx = 0; bucketIdx < map.BUCKET_COUNT; ++bucketIdx){ \
    const auto& bucket = map.buckets[bucketIdx]; \
    for(idx_t contentIdx = 0; contentIdx < bucket.count; ++contentIdx){ \
        const auto& item = bucket.content[contentIdx];
#define FOR_STRING_MAP_END }}


template<class VAL, auto STRING_CAPACITY>
struct StringMap
{
    using KEY_T = box::String<STRING_CAPACITY>;
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
        const auto hash = SimpleHash(pair.key.data);
        buckets[hash].Add(pair);
    }

    //? GET

    const VAL* GetOptional(chars_t key) const
    {
        const auto hash = SimpleHash(key);
        auto& bucket = buckets[hash];
        
        if (bucket.count == 0)
            return nullptr;

        FOR_CARRAY(bucket.content, i){
            auto& pair = bucket.content[i];
            if (pair.key == key) //str operator==
                return &pair.val;
        }
        return nullptr;
    }

    //? INTERNAL

    idx_t SimpleHash(chars_t str) const
    {
        const auto c1 = str[0];
        const auto c2 = str[utils::StrLen(str) - 1];
        return (c1 + c2) % BUCKET_COUNT;
    }

    idx_t SimpleHash(const utils::CharsView& view) const
    {
        dbg::Assert(view.len > 0, "chars view count is not greater than 0");
        const auto c1 = *(view.beginPtr);
        const auto c2 = *(view.beginPtr + view.len - 1);
        return (c1 + c2) % BUCKET_COUNT;
    }

};

template<class VAL, auto STRING_CAPACITY>
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

template<class VAL, auto STRING_CAPACITY, auto ENUM_END_T>
auto StringMapFromEnumMap(const box::EnumMap<ENUM_END_T, box::String<STRING_CAPACITY>>& enumMap)
{
    StringMap<VAL, STRING_CAPACITY> strMap;
    FOR_ENUM_MAP(enumMap, i){
        strMap.Insert({enumMap.Get(i), (VAL)i});
    }
    return strMap;
}

} //NS