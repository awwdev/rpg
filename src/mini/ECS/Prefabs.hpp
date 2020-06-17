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
        ENUM_END
    };

    const box::IndexMap<box::String<20>, PrefabType::ENUM_END> prefabTypeToStr
    {
        { PrefabType::UI_FpsMonitor,   "UI_FpsMonitor"   },
        { PrefabType::Foo,             "Foo"             },
    };

    const box::IndexMap<box::String<20>, KeyType::ENUM_END> keyTypeToStr 
    {
        { KeyType::prefab, "prefab" },
    };

    const box::IndexMap<box::String<20>, ComponentType::ENUM_END> componentTypeToStr
    {
        { ComponentType::Transform,  "transform"  },
    };


    inline ComponentType GetComponentType(const utils::CharsView& view)
    {
        FOR_INDEX_MAP_BEGIN(componentTypeToStr, i)
            if (utils::CharsCompare(view, componentTypeToStr.Get(i).dataPtr))
                return (ComponentType)i;
        FOR_INDEX_MAP_END
        WARN("str to enum: invalid component type");
        return ComponentType::ENUM_END;
    }

    inline KeyType GetKey(const utils::CharsView& view)
    {
        FOR_INDEX_MAP_BEGIN(keyTypeToStr, i)
            if (utils::CharsCompare(view, keyTypeToStr.Get(i).dataPtr))
                return (KeyType)i;
        FOR_INDEX_MAP_END
        //ENUM_END key can be a component
        //WARN("str to enum: invalid key type");
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

            /*
            #example of the text layout

            prefab:UI_FpsMonitor
            ui:42

            prefab:Foo
            ui
            */

            //! always check if max line count is enough
            //a line is key:value pair
            constexpr u32 LINE_CHARS_MAX = 30;
            char line[LINE_CHARS_MAX];
            PrefabType currentPrefab = PrefabType::ENUM_END;
            while (file.getline(line, LINE_CHARS_MAX)) 
            {
                LOG("line");
                if (line[0] == '\0' || line[0] == ' ') continue;
                
                u32 valueBegin = 0;
                KeyType currentKey = KeyType::ENUM_END;

                for(u32 i = 0; i < LINE_CHARS_MAX; ++i)
                {
                    if (line[i] == ':')
                    {
                        valueBegin = i + 1;
                    }
                    else if (line[i] == '\0')
                    {
                        currentKey = GetKey({ line, valueBegin == 0 ? i : valueBegin - 1 });
                        LOG("key", (int)currentKey);

                        if (currentKey == KeyType::prefab)
                        {
                            currentPrefab = GetPrefabType({ line + valueBegin, i - valueBegin }); 
                            LOG("prefab", (int)currentPrefab);
                            break;
                        }
                        else if (currentPrefab != PrefabType::ENUM_END)
                        {
                            //key type is ENUM_END for component
                            const auto componentType = GetComponentType({ line, valueBegin == 0 ? i : valueBegin - 1 });
                            if (componentType == ComponentType::ENUM_END)
                                break;

                            arrays.signatures[(u32)currentPrefab].Set<true>(componentType);
                            if (valueBegin != 0)
                            {
                                switch(componentType)
                                {
                                    case ComponentType::Transform:  LOG("parse component data"); break;
                                }
                            }
                           
                        }

                        break; //line finished
                    }



                    //GET KEY
                    //if (line[i] == ':' || (line[i] == '\0' && valueBegin == 0))
                    //{
                    //    currentKey = GetKey({ line, i });
                    //    LOG("key", (int)currentKey);
                    //    valueBegin = i + 1;
                    //}
                    // //GET VALUE
                    //else if (line[i] == '\0')
                    //{
                    //    //SET CURRENT PREFAB INDEX
                    //    if (currentKey == KeyType::prefab)
                    //    {
                    //        currentPrefab = GetPrefabType({ line + valueBegin, i - valueBegin }); 
                    //        LOG("prefab", (int)currentPrefab);
                    //    }
                    //    //PARSE COMPONENT DATA (if value and if valid prefab)
                    //    else if (currentPrefab != PrefabType::ENUM_END)
                    //    {
                    //        switch (currentKey)
                    //        {
                    //            case KeyType::ui: LOG("parse ui component"); break;
                    //            //TODO: more
                    //            //TODO: write component data parse
                    //        }
                    //    }
                    //}
                    ////END OF LINE PARSING
                    //if (line[i] == '\0')
                    //    break;
                }
            } 
        }

    };  

}//ns