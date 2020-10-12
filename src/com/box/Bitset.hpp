//https://github.com/awwdev

#pragma once
#include "com/Utils.hpp"
#include "dbg/Assert.hpp"
#include "dbg/Logger.hpp"
#include "com/box/Optional.hpp"

#include <cstring>
#include <fstream>

namespace rpg::com {

constexpr bool USE_BITSET_ASSERTS = true;

#define FOR_BITSET(bitset, i) \
for(idx_t i = 0; i < bitset.LastActiveBit(); ++i)

template<auto N>
struct Bitset
{
    static constexpr idx_t BIT_COUNT  = static_cast<idx_t>(N);
    static constexpr idx_t BYTE_COUNT = BIT_COUNT / 8 + 1;

    //? constructors

    constexpr 
    Bitset(idx_t const num = 0)
        : bytes {} //nullify
        , lastActiveBit {}
    {
        BitsetAssert(BytesNeeded(num) <= BYTE_COUNT, "[Bitset::Ctor] bitset ctor input to big");
        for(idx_t i = 0; i < BYTE_COUNT; ++i) 
        {
            const idx_t bits = i * 8;
            bytes[i] = static_cast<u8>((num >> bits) & 0xFF);
            if (bytes[i])
                ++lastActiveBit;
        }
    }

    //? methods

    void Set(const auto i, const bool setTrue = true)
    {
        BitsetAssert(static_cast<idx_t>(i) < BIT_COUNT, "[BitsetAssert::Set] idx out of bounds");

        bytes[Byte(i)] = setTrue
            ? bytes[Byte(i)] |  (1 << Bit(i))
            : bytes[Byte(i)] & ~(1 << Bit(i));

        CheckLastActiveBit(i, setTrue);
    }

    bool Test(const auto i) const
    {
        BitsetAssert((idx_t)i < BIT_COUNT, "[Bitset] idx out of bounds");
        return bytes[Byte(i)] & (1 << Bit(i));
    }

    void Flip(const auto i)
    {
        BitsetAssert(static_cast<idx_t>(i) < BIT_COUNT, "[Bitset] idx out of bounds");
        bytes[Byte(i)] ^= 1 << Bit(i);
        CheckLastActiveBit(i, bytes[Byte(i)]);
    }

    void Clear()
    {
        std::memset(bytes, 0, BYTE_COUNT);
        lastActiveBit = {};
    }

    auto LastActiveBit() const 
    {
        return lastActiveBit;
    }

    //? helper

    com::Optional<idx_t> FindFreeBit_Optional(const idx_t startAt = 0) const
    {
        for (idx_t i = startAt; i < BIT_COUNT; ++i) 
        {
            const auto a = bytes[Byte(i)] & (1 << Bit(i));
            if (a == 0) 
                return { i };
        }
        return {};
    }

    constexpr auto Decimal() const
    {
        std::size_t num = 0;
        for (auto i = 0; i < BYTE_COUNT; ++i)
        {
            num += (((std::size_t)bytes[i]) << (i*8));
        }            
        return num;
    }

    //? serialization

    void WriteBinaryFile(chars_t path) const
    {
        std::ofstream file { path, std::ios::binary };
        dbg::Assert(file.is_open(), "[IO] cannot open file"); //not an array assert
        file << lastActiveBit;
        file.write(reinterpret_cast<char const*>(bytes), BytesNeeded(lastActiveBit));
    }

    void ReadBinaryFile(chars_t path)
    {
        std::ifstream file { path, std::ios::binary | std::ios::ate };
        dbg::Assert(file.is_open(), "[IO] cannot open file"); //not an array assert
        auto size = file.tellg();
        file.seekg(std::ios::beg);
        file >> lastActiveBit;
        file.read(reinterpret_cast<char*>(bytes), size);
    }

private:

    //? internal helper

    void BitsetAssert(bool const expr, chars_t msg) const 
    {
        if constexpr (!USE_BITSET_ASSERTS) return;
        dbg::Assert(expr, msg);
    }

    constexpr idx_t Bit (auto const i) const { return static_cast<idx_t>(i) % idx_t { 8 }; }
    constexpr idx_t Byte(auto const i) const { return static_cast<idx_t>(i) / idx_t { 8 }; }

    constexpr idx_t BytesNeeded(idx_t num) const
    {
        idx_t n = 0;
        do { num >>= 8; n++; } while (num);
        return n;
    }

    constexpr void CheckLastActiveBit(auto const idx, bool const expr = true)
    {
        if (static_cast<idx_t>(idx) > lastActiveBit && expr)
            lastActiveBit = static_cast<idx_t>(idx);
    }

    //? data 
    u8 bytes [BYTE_COUNT];
    idx_t lastActiveBit;

};

}//ns