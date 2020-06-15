//https://github.com/awwdev

#pragma once
#include "mini/ECS/ComponentArray.hpp"
#include "mini/Box/IndexMap.hpp"
#include "mini/Box/String.hpp"
#include "mini/Memory/Allocator.hpp"
#include "mini/Utils/Utils.hpp"
#include "mini/Debug/Logger.hpp"

#include <fstream>


namespace mini::ecs
{
    enum class PrefabType
    {
        UI_FpsMonitor = 1,
        Foo,
        ENUM_END
    };

    namespace PrefabTypeToString
    {
        using M = box::IndexMap<box::String<20>, PrefabType::ENUM_END>;
        using P = M::Pair_t;
        const M map 
        {
            P{ PrefabType::UI_FpsMonitor,   "UI_FpsMonitor"   },
            P{ PrefabType::Foo,             "Foo2"            },
        };
    }
    
    struct Token
    {
        box::String<20> str;
    };

    inline void Tokenize(chars_t text, box::IArray<Token>& tokens)
    {
        u32 tokenBegin = 0;
        for(u32 i = 0; text[i] != '\0'; ++i)
        {
            if (text[i] == '\n' || text[i] == ':') {
                tokens.Append(Token { { text + tokenBegin, i - tokenBegin } });
                tokenBegin = i + 1; //exclude delimiter
            }
        }
    }

    struct Prefabs
    {
        ComponentArrays<(u32)PrefabType::ENUM_END> arrays;

        inline void Load(chars_t path)
        {
            std::ifstream file(path);
            auto ptrBuffer = mem::ClaimBlock<char[1000]>();
            file.read(*ptrBuffer, 1000);

            box::Array<Token, 20> tokens;
            Tokenize(*ptrBuffer, tokens);

            FOR_ARRAY(tokens, i) {
                LOG(tokens[i].str);
                //! for whatever reasons dataPtr is garbage of IString
            }
                
        }

    };  



}//ns

/*
FOR_INDEX_MAP_BEGIN(map, prefabID)
if (CharsCompare("prefab", ptrBuffer.Get() + start, i - start))
{
LOG("found token", "prefab");
break;
}
FOR_INDEX_MAP_END
*/

 /*
u32 start = 0;
FOR_CARRAY((*ptrBuffer), i)
{
if (ptrBuffer[i] == ':')
{
        if (CharsCompare("prefab", ptrBuffer.Get() + start, i - start)) {
        LOG("found token", "prefab");
        start = i + 1;
        for(; ptrBuffer[i] != '\n'; ++i);
        FOR_INDEX_MAP_BEGIN(PrefabTypeToString::map, prefabIdx)
            dbg::PrintCharRange(*ptrBuffer, start, i);
            LOG(PrefabTypeToString::map.Get(prefabIdx).dataPtr);
            if (CharsCompare(PrefabTypeToString::map.Get(prefabIdx).dataPtr, &ptrBuffer[start], i - start))
                LOG(prefabIdx);
        FOR_INDEX_MAP_END
    }
    start = i + 1;
}

if (ptrBuffer[i] == '\n'){
    start = i + 1;
}

if (ptrBuffer[i] == '\0'){
    break;
}
    
}
*/