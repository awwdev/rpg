//https://github.com/awwdev

#pragma once
#include "mini/ECS/ComponentArray.hpp"
#include "mini/Box/IndexMap.hpp"
#include "mini/Box/String.hpp"
#include "mini/Memory/Allocator.hpp"
#include "mini/Utils/Utils.hpp"

#include <fstream>


namespace mini::ecs
{
    enum class PrefabType
    {
        UI_FpsMonitor,
        Foo,
        ENUM_END
    };

    namespace PrefabTypeToString
    {
        using M = box::IndexMap<box::String<20>, PrefabType::ENUM_END>;
        using P = M::Pair_t;
        const M map 
        {
            P{ PrefabType::UI_FpsMonitor,   "UI_FpsMonitor"  },
            P{ PrefabType::Foo,             "Foo"            },
        };
    }
    

    struct Prefabs
    {
        ComponentArrays<(u32)PrefabType::ENUM_END> arrays;

        inline void Load(chars_t path)
        {
            std::ifstream file(path);
            
            auto ptrBuffer = mem::ClaimBlock<char[1000]>();
            file.read(*ptrBuffer, 1000);

            u32 start = 0;
            FOR_CARRAY((*ptrBuffer), i)
            {
                //delimiters
                if (ptrBuffer[i] == '\0' || 
                    ptrBuffer[i] == ':'  ||
                    ptrBuffer[i] == '\n')
                {
                    dbg::PrintCharRange(*ptrBuffer, start, i);

                    using namespace PrefabTypeToString;
                    FOR_INDEX_MAP_BEGIN(map, j)
                        LOG(map.Get(j).dataPtr);
                        if (const auto cmp = CharsCompare(map.Get(j).dataPtr, ptrBuffer.Get() + start, i - start))
                            LOG("found");
                    FOR_INDEX_MAP_END

                    start = i + 1;

                    if (ptrBuffer[i] == '\0')
                        break;
                }

            }

        }

    };    

}//ns