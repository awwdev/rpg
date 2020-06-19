//https://github.com/awwdev

#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Debug/Logger.hpp"

namespace mini::box
{
    template<class VALUE, u8 BUCKET_COUNT_T = 20, u8 BUCKET_LENGTH_T = 20>
    struct StringMap
    {
        constexpr static auto BUCKET_COUNT  = BUCKET_COUNT_T;
        constexpr static auto BUCKET_LENGTH = BUCKET_LENGTH_T;

        struct Pair 
        {
            constexpr static auto STRING_CAPACITY = 20;

            char str[STRING_CAPACITY]; //capacity
            VALUE  val;

            constexpr Pair() = default;

            template<u8 SIZE>
            constexpr Pair(const char(&pStr)[SIZE], const VALUE pVal) 
                : str { }
                , val { pVal } 
            {
                static_assert(SIZE <= STRING_CAPACITY);
                for(u8 i = 0; i < SIZE; ++i)
                    str[i] = pStr[i];
            }

            constexpr void operator=(const Pair& other)
            { 
                val = other.val;
                for(u8 i = 0; i < STRING_CAPACITY; ++i)
                    str[i] = other.str[i];
            }
        };

        struct Bucket
        {
            u8 count = 0;
            Pair content[BUCKET_LENGTH];

            constexpr Bucket() : content {} {}
            constexpr void Add(const Pair& pair)
            {
                content[count] = pair;
                ++count;
            }
        };

        constexpr u8 Hash(chars_t str) const
        {
            return (str[0] + (str[1] != '\0' ? str[1] : 0)) % BUCKET_COUNT;
        }


        constexpr StringMap() : buckets {} {}
        constexpr StringMap(std::initializer_list<Pair> pairs) : StringMap()
        {
            for(const auto& pair : pairs)
            {
                const auto hash = Hash(pair.str);
                buckets[hash].Add(pair);
            }
        }

        Bucket buckets[BUCKET_COUNT];
    };

    template<class VALUE>
    inline void PrintStringMap(const StringMap<VALUE>& map)
    {
        for(u8 bucketIdx = 0; bucketIdx < map.BUCKET_COUNT; ++bucketIdx)
        {
            const auto& bucket = map.buckets[bucketIdx];
            for(u8 contentIdx = 0; contentIdx < bucket.count; ++contentIdx)
            {
                LOG((int)bucketIdx, bucket.content[contentIdx].val);
            }
        }
    }

}//ns