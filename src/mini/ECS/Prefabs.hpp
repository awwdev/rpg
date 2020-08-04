//https://github.com/awwdev

#pragma once
#include "mini/ECS/ComponentArray.hpp"
#include "mini/Box/EnumMap.hpp"
#include "mini/Box/StringMap.hpp"
#include "mini/Box/String.hpp"
#include "mini/Utils/Algorithms.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Utils/CharsView.hpp"

#include <fstream>
#include <cstdlib>


namespace mini::ecs
{
    //?text layout: MiniFormat
    /*
    PREFAB:UI_FpsMonitor
    COMPONENT:UI
    pos:20,20
    COMPONENT:XYZ
    data1
    data2:123

    #basically key value pairs PER LINE (delimiter is :)
    #there can be key only PER LINE (empty component)
    #while PREFAB and COMPONENT are named keys, COMPONENT_DATA is implicit (no need to write it everytime)
    #values can have additional delimiter (,) for arrays/vecs
    #watch spaces and cases
    */

   //!atoi could be UB here

    enum class PrefabType
    {
        UI_FpsMonitor,
        UI_Button,
        ENUM_END
    };

    const box::StringMap<PrefabType> prefabTypeToStr
    {
        { "UI_FpsMonitor",   PrefabType::UI_FpsMonitor  },
        { "UI_Button",       PrefabType::UI_Button      },
    };

    enum class KeyType
    {
        PREFAB,
        COMPONENT,
        COMPONENT_DATA, //no str (internal use)
        ENUM_END
    };

    const box::StringMap<KeyType> keyTypeToStr 
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

    //? PARSE COMMA SEPERATED DATA

    inline auto ParseRect(utils::CharsView view)
    {
        utils::Rect<int> rect;
        rect.x = std::atoi(view.beginPtr);
        while(*view.beginPtr != '|') { ++(view.beginPtr); }
        rect.y = std::atoi(++view.beginPtr);
        while(*view.beginPtr != '|') { ++(view.beginPtr); }
        rect.w = std::atoi(++view.beginPtr);
        while(*view.beginPtr != '|') { ++(view.beginPtr); }
        rect.h = std::atoi(++view.beginPtr);
        //if access violation you probably increase the ptr to much when there is no delimiter anymore
        //check for null terminating
        return rect;
    }

    template<class VECTOR>
    inline VECTOR ParseVector(utils::CharsView view)
    {
        VECTOR vec;
        for(auto x = 0; x < VECTOR::X; ++x)
        {
            vec[0][x] = std::atoi(view.beginPtr); 
            while(*view.beginPtr != '|' && *view.beginPtr != '\0') { ++(view.beginPtr); }
            ++view.beginPtr;
        }
        return vec;
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
                if (line[0] == '\0' || line[0] == ' ') continue;
                
                //per line validity (so reset)
                u32 valueBegin = 0; //there can be lines without values (key only)
                KeyType currentKey = KeyType::ENUM_END; 

                for(u32 i = 0; i < LINE_CHARS_MAX; ++i)
                {
                    if (line[i] == ':')
                    {
                        valueBegin = i + 1; //value found
                    }

                    //?LINE END
                    else if (line[i] == '\0')
                    {
                        currentKey = GetKey({ line, valueBegin == 0 ? i : valueBegin - 1 });

                        if (currentKey == KeyType::PREFAB)
                        {
                            if (valueBegin == 0) 
                            {
                                WARN("key prefab without value!");
                                break;
                            }
                            currentPrefab = GetPrefabType({ line + valueBegin, i - valueBegin }); 
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
                                //add default component (and customize it when we actaully have a key see below)
                                //arrays.AddComponent((ID)currentPrefab, currentComponent);
                                break;
                            }
                            
                            if (currentKey == KeyType::COMPONENT_DATA) //implicit key
                            {
                                //ParseComponentData(currentComponent, { line, valueBegin - 1 }, { line + valueBegin, i - valueBegin });
                            }
                        }
                        
                        break; //!line finished
                    }
                }
            } 
        }

        /*
        void ParseComponentData(const ComponentType& componentType, const utils::CharsView& key, const utils::CharsView& value)
        {
            //the name of a component data could be the same for multiple component types
            const auto dataType = GetComponentDataType(key);
            auto& uiData = arrays.uiData.dense.Last();

            switch(dataType)
            {
                //TODO: box or rect? decide!
                case ComponentData::box:   
                {
                    const auto rect = ParseRect(value);
                    uiData.rect = rect;
                } 
                break;

                case ComponentData::text:   
                {
                    uiData.text.Set(value.beginPtr);
                } 
                break;

                case ComponentData::type:   
                {
                    uiData.type = [&]
                    {
                        if (utils::CharsCompare(value, "Button")) return C_UI::Type::Button;
                        if (utils::CharsCompare(value, "Label"))  return C_UI::Type::Label;
                        return C_UI::Type::Label; //not impl yet
                    }();
                } 
                break;

                case ComponentData::box_col:   
                {
                    uiData.rect_col = ParseVector<utils::RGBAColor4u>(value);
                } 
                break;

                case ComponentData::text_col:   
                {
                    uiData.text_col = ParseVector<utils::RGBAColor4u>(value);
                } 
                break;
            }
        }
        */

    };  

}//ns