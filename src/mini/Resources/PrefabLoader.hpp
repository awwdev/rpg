//https://github.com/awwdev

#pragma once
#include "mini/ECS/PrefabTypes.hpp"
#include "mini/ECS/ComponentArray.hpp"
#include "mini/Debug/Assert.hpp"

#include <fstream>

namespace mini::res {

constexpr auto LINE_CHARS_MAX = 100;
using ComponentDataStrings = box::Array<box::String<LINE_CHARS_MAX>, ecs::COMPONENT_DATA_COUNT_MAX>;

template<auto MAX_COUNT>
void LoadPrefabs(chars_t path, ecs::ComponentArrays<MAX_COUNT>& componentArrays)
{
    std::ifstream file(path, std::ios::beg);
    dbg::Assert(file.is_open(), "cannot open file");

    ComponentDataStrings intermediateData [ecs::PrefabType::ENUM_END][ecs::ComponentType::ENUM_END];    

    auto currentPrefab    = ecs::PrefabType::ENUM_END;
    auto currentComponent = ecs::ComponentType::ENUM_END;

    char line [LINE_CHARS_MAX];
    while(file.getline(line, LINE_CHARS_MAX))  
    {
        //? LINE TYPES
        enum LineType { Comment, PrefabName, ComponentName, ComponentData, Empty, Invalid } lineType;
        constexpr auto IDENTIFIER_START = 2;

        switch(line[0])
        {
            case '#': lineType = Comment;       break;
            case '@': lineType = PrefabName;    break;
            case '?': lineType = ComponentName; break;
            case '+': lineType = ComponentData; break;

            case '\0': case '\n': case ' ':  
            lineType = Empty; break;

            default: lineType = Invalid;
        }

        //? HOW TO DEAL WITH LINES
        if (lineType == Invalid)
            dbg::LogWarning("bad parsing");

        if (lineType == Empty || lineType == Comment || lineType == Invalid)
            continue;

        //? PREFAB
        if (lineType == PrefabName){
            if (auto prefabType = ecs::PREFAB_STR_TO_ENUM.GetOptional(&line[IDENTIFIER_START])){
                currentPrefab = *prefabType;
                currentComponent = ecs::ComponentType::ENUM_END; //reset
                continue;
            }
        }
        if (currentPrefab == ecs::PrefabType::ENUM_END){
            dbg::LogWarning("no prefab type for parsing");
            continue;
        }

        //? COMPONENT
        if (lineType == ComponentName){
            if (auto componentType = ecs::COMPONENT_STR_TO_ENUM.GetOptional(&line[IDENTIFIER_START])){
                currentComponent = *componentType;
                continue;
            }
        }   
        if (currentComponent == ecs::ComponentType::ENUM_END){
            dbg::LogWarning("no component type for parsing");
            continue;
        }

        if (lineType == ComponentData){
            intermediateData[(idx_t)currentPrefab][(idx_t)currentComponent].Append(&line[IDENTIFIER_START]);
        }

    }

    PrintParsedData(intermediateData);

}

inline void PrintParsedData(const ComponentDataStrings (&intermediateData) [ecs::PrefabType::ENUM_END][ecs::ComponentType::ENUM_END])
{
    for(idx_t p = 0; p < (idx_t)ecs::PrefabType::ENUM_END; ++p) {
        dbg::LogInfo("PrefabType", ecs::PREFAB_ENUM_TO_STR.Get(p));
        for(idx_t c = 0; c < (idx_t)ecs::ComponentType::ENUM_END; ++c) {
            dbg::LogInfo("ComponentType", ecs::COMPONENT_ENUM_TO_STR.Get(c));
            const auto& arr = intermediateData[p][c];
            FOR_ARRAY(arr, i){
                dbg::LogInfo(arr[i]);
            }
        }
    }
}

}//NS