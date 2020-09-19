//https://github.com/awwdev

#pragma once
#include "ECS/EntityID.hpp"
#include "ECS/Prefabs/PrefabTypes.hpp"
#include "ECS/Components/Meta/ComponentArray.hpp"
#include "Debug/Assert.hpp"
#include "Common/Memory/Allocator.hpp"

#include <fstream>

namespace rpg::res {

template<auto MAX_COUNT>
void LoadPrefabs(chars_t path, ecs::ComponentArrays<MAX_COUNT>& componentArrays)
{
    using namespace ecs;

    std::ifstream file(path);//, std::ios::beg
    dbg::Assert(file.is_open(), "cannot open file");

    struct Arr { ComponentDataStringPairs data [(idx_t)PrefabType::ENUM_END][(idx_t)ComponentType::ENUM_END]; };
    auto blockPtrArr = rpg::com::mem::ClaimBlock<Arr>();
    auto& componentDataKeyValueArray = blockPtrArr->data;

    auto currentPrefab    = PrefabType::ENUM_END;
    auto currentComponent = ComponentType::ENUM_END;

    //? META PARSE
    char line [COMPONENT_MAX_STR_LEN];
    while(file.getline(line, COMPONENT_MAX_STR_LEN))  
    {
        enum LineType { Comment, PrefabName, ComponentName, ComponentData, Empty, Invalid } lineType;

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
            if (auto prefabType = PREFAB_STR_TO_ENUM.GetOptional(&line[IDENTIFIER_START])){
                currentPrefab = *prefabType;
                currentComponent = ComponentType::ENUM_END; //reset
                continue;
            }
        }
        if (currentPrefab == PrefabType::ENUM_END){
            dbg::LogWarning("no prefab type for parsing");
            continue;
        }

        //? COMPONENT
        if (lineType == ComponentName){
            if (auto componentType = COMPONENT_STR_TO_ENUM.GetOptional(&line[IDENTIFIER_START])){
                currentComponent = *componentType;
                continue;
            }
        }   
        if (currentComponent == ComponentType::ENUM_END){
            dbg::LogWarning("no component type for parsing");
            continue;
        }

        //? COMPONENT DATA
        if (lineType == ComponentData){
            const auto pair = ConvertToComponentDataStringPair(line);
            componentDataKeyValueArray[(idx_t)currentPrefab][(idx_t)currentComponent].Append(pair);
        }

    }

    //? INSTANTIATION AND COMPONENT DATA PARSING
    //PrintParsedData(componentDataKeyValueArray);
    for(idx_t p = 0; p < (idx_t)PrefabType::ENUM_END; ++p) {
    for(idx_t c = 0; c < (idx_t)ComponentType::ENUM_END; ++c) {
        const auto& pairs = componentDataKeyValueArray[p][c];
        if (pairs.Empty() == false)
            componentArrays.AddComponent((ID)p, (ComponentType)c, pairs);
    }}

}

inline void PrintParsedData(
    const ecs::ComponentDataStringPairs (&componentDataKeyValueArray) [(idx_t)ecs::PrefabType::ENUM_END][(idx_t)ecs::ComponentType::ENUM_END])
{
    using namespace ecs;

    for(idx_t p = 0; p < (idx_t)PrefabType::ENUM_END; ++p) {
        dbg::LogInfo("PrefabType", PREFAB_ENUM_TO_STR.Get(p));
        for(idx_t c = 0; c < (idx_t)ComponentType::ENUM_END; ++c) {
            dbg::LogInfo("ComponentType", COMPONENT_ENUM_TO_STR.Get(c));
            const auto& arr = componentDataKeyValueArray[p][c];
            FOR_ARRAY(arr, i){
                dbg::LogInfo(arr[i].key, arr[i].val);
            }
        }
    }
}

}//NS