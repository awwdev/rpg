//https://github.com/awwdev

#pragma once
#include "mini/ECS/ComponentArray.hpp"
#include "mini/Box/IndexMap.hpp"
#include "mini/Box/StringMap.hpp"
#include "mini/Box/String.hpp"
#include "mini/Memory/Allocator.hpp"
#include "mini/Utils/Utils.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Utils/CharsView.hpp"

#include <fstream>


namespace mini::ecs
{
    //?text layout: MiniFormat
    /*
    PREFAB:Foo1
    COMPONENT:Transform
    data1:0
    COMPONENT:RenderData
    data1:42
    data2

    PREFAB:Foo2
    COMPONENT:Transform
    data1
    data2

    #basically key value pairs PER LINE (delimiter is :)
    #there can be key only PER LINE
    #while PREFAB and COMPONENT are named keys, COMPONENT_DATA is implicit (no need to write it everytime)
    #values can have additional delimiter (,) for arrays/vecs
    #watch spaces and cases
    */

    enum class PrefabType
    {
        Foo1,
        Foo2,
        ENUM_END
    };

    constexpr box::StringMap<PrefabType> prefabTypeToStr
    {
        { "Foo1",   PrefabType::Foo1 },
        { "Foo2",   PrefabType::Foo2 },
    };

    enum class KeyType
    {
        PREFAB,
        COMPONENT,
        COMPONENT_DATA, //no str (internal use)
        ENUM_END
    };

    constexpr box::StringMap<KeyType> keyTypeToStr 
    {
        { "PREFAB",     KeyType::PREFAB     },
        { "COMPONENT",  KeyType::COMPONENT  },
    };


    inline KeyType GetKey(const utils::CharsView& view)
    {
        const auto* const key = keyTypeToStr.GetOptional(view);
        if (key) return *key;

        if (GetComponentDataType(view) != ComponentData::ENUM_END)
            return KeyType::COMPONENT_DATA;

        WARN("str to enum: invalid key type");
        return KeyType::ENUM_END;
    }

    inline PrefabType GetPrefabType(const utils::CharsView& view)
    {
        const auto* const  value = prefabTypeToStr.GetOptional(view);
        if (value) return *value;
        WARN("str to enum: invalid prefab type");
        return PrefabType::ENUM_END;
    }

    inline void ParseComponentData(const utils::CharsView& key, const utils::CharsView& value)
    {
        const auto type = GetComponentDataType(key);
        switch(type)
        {
            case ComponentData::pos:    LOG("parse component pos");   break;
            case ComponentData::data1:  LOG("parse component data1"); break;
            case ComponentData::data2:  LOG("parse component data2"); break;
        }
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

            //multiline validity
            PrefabType currentPrefab = PrefabType::ENUM_END;   
            ComponentType currentComponent = ComponentType::ENUM_END;

            //? "TOKENIZING"
            while (file.getline(line, LINE_CHARS_MAX)) 
            {
                LOG("line");
                if (line[0] == '\0' || line[0] == ' ') continue;
                
                //per line validity (so reset)
                u32 valueBegin = 0; //there can be lines without values (key only)
                KeyType currentKey = KeyType::ENUM_END; 

                for(u32 i = 0; i < LINE_CHARS_MAX; ++i)
                {
                    //?VALUE FOUND
                    if (line[i] == ':')
                    {
                        valueBegin = i + 1;
                    }
                    //?LINE END
                    else if (line[i] == '\0')
                    {
                        //? GET THE KEY
                        currentKey = GetKey({ line, valueBegin == 0 ? i : valueBegin - 1 });
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
                                arrays.signatures[(u32)currentPrefab].Set<true>(currentComponent);
                                LOG("component", (int)currentComponent);
                                break;
                            }
                            
                            if (currentKey == KeyType::COMPONENT_DATA)
                            {
                                ParseComponentData({ line, valueBegin - 1 }, { line + valueBegin, i - valueBegin });
                            }
                        }
                        
                        break; //!line finished
                    }
                }
            } 
        }

    };  

}//ns

//TODO: currently we have string from text, and then we iterate WHOLE map and do matching to eventually get the enum ...
//it would be better to have the map reversed, where the string is the index
//this means we need a hashmap (str gets transformed into int), and then we check if index exists

//IDEA: fixed buckets, simple hash function, when you add something to the map it gets hased too
//and the index is stored in that array
//when you call contains you hash again, get the bucket (and then iterate the bucket only, could have multiple hashes)

//template<class T> int hash(const T&) -> BUCKET that stores indices
//sizeof(T) % 10

//USE THIS FOR ADDITIONAL PERFORMANCE ON INDEX MAP 
// (*str % 10)

//insertion where string is key however would be better but also more intense
