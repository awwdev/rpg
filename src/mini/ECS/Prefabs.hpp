//https://github.com/awwdev

#pragma once
#include "mini/ECS/ComponentArray.hpp"
#include "mini/Box/IndexMap.hpp"
#include "mini/Box/String.hpp"
#include "mini/Memory/Allocator.hpp"
#include "mini/Utils/Utils.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Utils/CharsView.hpp"

#include <fstream>


namespace mini::ecs
{
    enum class PrefabType
    {
        UI_FpsMonitor,
        Foo,
        ENUM_END
    };

    enum class KeyType
    {
        prefab,
        ui,
        foo,
        ENUM_END
    };

    namespace PrefabTypeToString
    {
        using M = box::IndexMap<box::String<20>, PrefabType::ENUM_END>;
        using P = M::Pair_t;
        const M map 
        {
            P{ PrefabType::UI_FpsMonitor,   "UI_FpsMonitor"   },
            P{ PrefabType::Foo,             "Foo"             },
        };
    }

    namespace KeyTypeToString
    {
        using M = box::IndexMap<box::String<20>, KeyType::ENUM_END>;
        using P = M::Pair_t;
        const M map 
        {
            P{ KeyType::prefab,     "prefab" },
            P{ KeyType::ui,         "ui" },
        };
    }
    
    
    inline KeyType GetKey(const utils::CharsView& view)
    {
        using namespace KeyTypeToString;
        FOR_INDEX_MAP_BEGIN(map, i)
            if (utils::CharsCompare(view, map.Get(i).dataPtr))
                return (KeyType)i;
        FOR_INDEX_MAP_END
        WARN("parsing: invalid key type");
        return KeyType::ENUM_END;
    }

    inline PrefabType GetPrefabType(const utils::CharsView& view)
    {
        using namespace PrefabTypeToString;
        FOR_INDEX_MAP_BEGIN(map, i)
            if (utils::CharsCompare(view, map.Get(i).dataPtr))
                return (PrefabType)i;
        FOR_INDEX_MAP_END
        WARN("parsing: invalid prefab type");
        return PrefabType::ENUM_END;
    }


    struct Prefabs
    {
        ComponentArrays<(u32)PrefabType::ENUM_END> arrays;

        inline void Load(chars_t path)
        {
            std::ifstream file(path, std::ios::beg);

            //! always check if max line count is enough
            constexpr u32 LINE_CHARS_MAX = 30;
            char line[LINE_CHARS_MAX];
            //a line is key:value pair
            PrefabType currentPrefab = PrefabType::ENUM_END;
            while (file.getline(line, LINE_CHARS_MAX)) 
            {
                LOG("line");
                if (line[0] == '\0' || line[0] == ' ') continue;
                
                u32 valueBegin = 0;
                KeyType currentKey = KeyType::ENUM_END;

                for(u32 i = 0; i < LINE_CHARS_MAX; ++i)
                {
                    //GET KEY
                    if (line[i] == ':' || (line[i] == '\0' && valueBegin == 0))
                    {
                        currentKey = GetKey({ line, i });
                        LOG("key", (int)currentKey);
                        valueBegin = i + 1;
                    }
                    //GET VALUE
                    else if (line[i] == '\0')
                    {
                        //SET CURRENT PREFAB INDEX
                        if (currentKey == KeyType::prefab)
                        {
                            currentPrefab = GetPrefabType({ line + valueBegin, i - valueBegin }); 
                            LOG("prefab", (int)currentPrefab);
                        }
                        //PARSE COMPONENTS
                        else if (currentPrefab != PrefabType::ENUM_END)
                        {
                            switch (currentKey)
                            {
                                case KeyType::ui: LOG("parse ui component"); break;
                                //TODO: more
                            }
                        }
                    }
                    //END OF LINE PARSING
                    if (line[i] == '\0')
                        break;
                }
            } 
        }

    };  

}//ns