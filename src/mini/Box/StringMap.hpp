//https://github.com/awwdev

#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Debug/Assert.hpp"
#include "mini/Utils/CharsView.hpp"

namespace mini::box
{
    constexpr bool AreStringsEqual(const char* a, const char* b, u8 count)
    {
        for(u8 i = 0; i < count; ++i)
        {
            if (*a != *b) return false;
            ++a; ++b;
        }
        return true;
    }

    constexpr bool AreStringsEqual(const char* a, const char* b)
    {
        while (*a && *a == *b) { ++a; ++b; }
        return *a == *b;
    }

    template<class VALUE>
    struct StringMap
    {
        constexpr static auto BUCKET_COUNT    = 20;
        constexpr static auto BUCKET_LENGTH   = 20;
        constexpr static auto STRING_CAPACITY = 20;

        struct Pair 
        {
            char   str[STRING_CAPACITY]; //key
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
            u8   count = 0;
            Pair content[BUCKET_LENGTH] {};

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

        constexpr StringMap(std::initializer_list<Pair> pairs)
        {
            for(const auto& pair : pairs) {
                const auto hash = Hash(pair.str);
                if (GetOptional(pair.str) != nullptr) {
                    //!on compile time you get this error:
                    //!error C2131: expression did not evaluate to a constant
                    dbg::LogWarning("StringMap: key already exists (needs unique key)");
                }       
                buckets[hash].Add(pair);
            }
        }

        constexpr const VALUE* GetOptional(chars_t str) const
        {
            const auto hash = Hash(str);
            const auto& bucket = buckets[hash];
            for(u8 contentIdx = 0; contentIdx < bucket.count; ++contentIdx) {
                if (AreStringsEqual(bucket.content[contentIdx].str, str))
                    return &(bucket.content[contentIdx].val);
            }
            return nullptr;
        }

        constexpr const VALUE* GetOptional(const utils::CharsView& view) const
        {
            const auto hash = Hash(view.beginPtr);
            const auto& bucket = buckets[hash];
            for(u8 contentIdx = 0; contentIdx < bucket.count; ++contentIdx) {
                if (AreStringsEqual(bucket.content[contentIdx].str, view.beginPtr, view.count))
                    return &(bucket.content[contentIdx].val);
            }
            return nullptr;
        }

        //const VALUE& Get(chars_t str) const
        //{
        //    const auto hash = Hash(str);
        //    const auto& bucket = buckets[hash];
        //    for(u8 contentIdx = 0; contentIdx < bucket.count; ++contentIdx) {
        //        if (AreStringsEqual(bucket.content[contentIdx].str, str))
        //            return &ucket.content[contentIdx].val);
        //    }
        //    Assert(false, "Key not found but expected");
        //}

        //TODO: non const version

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
                dbg::LogInfo((int)bucketIdx, bucket.content[contentIdx].val, bucket.content[contentIdx].str);
            }
        }
    }

}//ns