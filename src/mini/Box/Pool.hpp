#pragma once
#include "mini/Box/Bitset.hpp"
#include "mini/Box/Array.hpp"

namespace mini::box
{

#define FOR_POOL(pool, i) for(auto i = 0; i < pool.COUNT; ++i)

    template<class T, auto N>
    struct Pool
    {
        static constexpr auto COUNT = N;

        box::Bitset<N> bitset;
        box::Array<T, N> data;

        T&       operator[](const decltype(N) idx)       { return data[idx]; }
        const T& operator[](const decltype(N) idx) const { return data[idx]; }

        Pool()
        {
            data.SetCompleteArray();
        }

        //this method does not flag the object as used! (use Set then)
        decltype(N) Get() const
        {
            const auto freeBit = bitset.FindFirstFreeBit();

            if (freeBit == bitset.BITS) {
                LOG("pool exhausted");
                return 0;
            }

            return freeBit;
        }

        template<bool SET>
        void Set(const decltype(N) idx)
        {
            bitset.Set<SET>(idx);
        }
    };


}//ns