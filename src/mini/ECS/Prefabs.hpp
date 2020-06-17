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
    //?text layout:
    /*
    PREFAB:Foo
    COMPONENT:Transform
    pos:0,0
    rot:180
    scale:1,1,2
    */

    enum class PrefabType
    {
        Foo1,
        Foo2,
        ENUM_END
    };

    enum class KeyType
    {
        PREFAB,
        COMPONENT,
        ENUM_END
    };

    const box::IndexMap<box::String<20>, PrefabType::ENUM_END> prefabTypeToStr
    {
        { PrefabType::Foo1,   "Foo1"   },
        { PrefabType::Foo2,   "Foo2"   },
    };

    const box::IndexMap<box::String<20>, KeyType::ENUM_END> keyTypeToStr 
    {
        { KeyType::PREFAB,      "PREFAB"    },
        { KeyType::COMPONENT,   "COMPONENT" },
    };

    //? GetComponentType is located in ComponentArray.hpp

    inline KeyType GetKey(const utils::CharsView& view)
    {
        FOR_INDEX_MAP_BEGIN(keyTypeToStr, i)
            if (utils::CharsCompare(view, keyTypeToStr.Get(i).dataPtr))
                return (KeyType)i;
        FOR_INDEX_MAP_END
        WARN("str to enum: invalid key type");
        return KeyType::ENUM_END;
    }

    inline PrefabType GetPrefabType(const utils::CharsView& view)
    {
        FOR_INDEX_MAP_BEGIN(prefabTypeToStr, i)
            if (utils::CharsCompare(view, prefabTypeToStr.Get(i).dataPtr))
                return (PrefabType)i;
        FOR_INDEX_MAP_END
        WARN("str to enum: invalid prefab type");
        return PrefabType::ENUM_END;
    }

    struct Prefabs
    {
        ComponentArrays<(u32)PrefabType::ENUM_END> arrays;

        inline void Parse(chars_t path)
        {
            std::ifstream file(path, std::ios::beg);

            //! always check if max line count is enough
            //a line is key:value pair
            constexpr u32 LINE_CHARS_MAX = 30;
            char line[LINE_CHARS_MAX];

            PrefabType      currentPrefab    = PrefabType::ENUM_END;
            ComponentType   currentComponent = ComponentType::ENUM_END;
            KeyType         currentKey       = KeyType::ENUM_END;

            while (file.getline(line, LINE_CHARS_MAX)) 
            {
                LOG("line");
                if (line[0] == '\0' || line[0] == ' ') continue;
                
                u32 valueBegin = 0; //there can be lines without values (key only)

                for(u32 i = 0; i < LINE_CHARS_MAX; ++i)
                {
                    //?VALUE FOUND
                    if (line[i] == ':')
                    {
                        valueBegin = i + 1;
                    }
                    //?AT LINE END
                    else if (line[i] == '\0')
                    {
                        //TODO: get key vs get component - depends if we are nested in a component key 
                        currentKey = GetKey({ line, valueBegin == 0 ? i : valueBegin - 1 }); //whole line can be a key
                        LOG("key", (int)currentKey);

                        if (currentKey == KeyType::PREFAB)
                        {
                            if (valueBegin == 0) 
                            {
                                WARN("key prefab without value!");
                                break;
                            }
                            currentPrefab = GetPrefabType({ line + valueBegin, i - valueBegin }); 
                            LOG("prefab", (int)currentPrefab);
                            break;
                        }

                        if (currentPrefab != PrefabType::ENUM_END)
                        {
                            if (currentKey == KeyType::COMPONENT)
                            {
                                if (valueBegin == 0) 
                                {
                                    WARN("key component without value!");
                                    break;
                                }
                                currentComponent = GetComponentType({ line + valueBegin, i - valueBegin }); 
                                LOG("component", (int)currentComponent);
                                arrays.signatures[(u32)currentPrefab].Set<true>(currentComponent);
                            }

                            //TODO:parse lines
                        }
                        
                        break; //!line finished
                    }
                }
            } 
        }

    };  

}//ns