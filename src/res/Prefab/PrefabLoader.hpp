//https://github.com/awwdev

#pragma once
#include "ecs/ComponentsMeta/ComponentArrays.hpp"
#include "ecs/ComponentsMeta/ComponentEnum.hpp"
#include "ecs/ComponentsMeta/ComponentData.hpp"
#include "res/Prefab/PrefabEnum.hpp"
#include "res/Prefab/PrefabMeta.hpp"
#include "com/mem/Allocator.hpp"
#include "dbg/Assert.hpp"

namespace rpg::res {

template<auto N>
inline void LoadPrefabs(chars_t path,  ecs::ComponentArrays<N>& prefabComponentArrays)
{
    std::ifstream file(path);
    dbg::Assert(file.is_open(), "cannot open file");

    //store all component data values per prefab
    struct Arr { ecs::ComponentDataPairs data [(idx_t) ecs::ComponentEnum::ENUM_END]; };
    auto  ptrComponentData = com::mem::ClaimBlock<Arr>();
    auto& componentData = ptrComponentData->data;

    constexpr auto NO_CURRENT_PREFAB    = PrefabEnum::ENUM_END;
    constexpr auto NO_CURRENT_COMPONENT = ecs::ComponentEnum::ENUM_END;
    auto currentPrefab    = NO_CURRENT_PREFAB;
    auto currentComponent = NO_CURRENT_COMPONENT;


    auto const NewPrefabFn = [&](){
        if (currentPrefab == NO_CURRENT_PREFAB) 
            return;

        FOR_C_ARRAY(componentData, componentEnumIdx) {
            const auto& pairs = componentData[componentEnumIdx];
            if (pairs.Empty())
                continue;
            prefabComponentArrays.SetComponent((ecs::ID) currentPrefab, (ecs::ComponentEnum) componentEnumIdx, pairs);
        }

        FOR_C_ARRAY(componentData, i)
            componentData[i].Clear();

        //give prefabs the prefab enum name
        auto const& prefabName = res::PREFAB_ENUM_TO_STR.Get(currentPrefab);
        const ecs::NameComponent nameComponent { prefabName.Data(), prefabName.Length() };
        prefabComponentArrays.nameComponents.SetComponent((ecs::ID) currentPrefab, nameComponent);
    };

    //line parsing
    char line [PREFAB_FILE_LINE_LEN_MAX + 1] {}; //last will always be '\0'
    while(file.getline(line, PREFAB_FILE_LINE_LEN_MAX))  
    {
        //clean-up line
        auto const lineLength = std::strlen(line);
        for(std::size_t i = 0; line[i] != '\0';) {
            if (std::isgraph (line[i]) == false) {
                std::memmove(&line[i], &line[i + 1], lineLength - i);
                //"continuous swapping" might be better
                continue;
            }   
            ++i;
        }

        //skip empty line
        if (std::strlen(line) == 0)
            continue;

        enum class LineEnum 
        { 
            Comment, 
            Prefab, 
            Component,
            ComponentData,
            Invalid
        };

        //prefab language signifiers 
        const auto lineEnum = [&line] {
            switch(line[0]) {
                case '#': return LineEnum::Comment;       
                case '@': return LineEnum::Prefab;    
                case '?': return LineEnum::Component; 
                case '+': return LineEnum::ComponentData; 
                default : return LineEnum::Invalid; 
            }
        }();
        dbg::Assert(lineEnum != LineEnum::Invalid, "invalid line");

        //remove signifier
        std::memmove(&line[0], &line[1], lineLength);

        if (lineEnum == LineEnum::Comment)
            continue;

        if (lineEnum == LineEnum::Prefab)   
        {
            auto prefabEnum = PREFAB_STR_TO_ENUM.GetOptional(line);
            dbg::Assert(prefabEnum, "no prefab enum");
            NewPrefabFn();          
            currentPrefab = *prefabEnum;
            continue;
        } 

        if (lineEnum == LineEnum::Component)   
        {
            auto componentEnum = ecs::COMPONENT_STR_TO_ENUM.GetOptional(line);
            dbg::Assert(componentEnum, "no component enum found");
            currentComponent = *componentEnum;
            continue;
        } 

        if (lineEnum == LineEnum::ComponentData)   
        {
            dbg::Assert(currentComponent != NO_CURRENT_COMPONENT, "no current component");
            auto const pair = ecs::LineStrToComponentDataPair(line);
            componentData[(idx_t) currentComponent].AppendElement(pair);
            continue;
        } 
        
    }

    NewPrefabFn(); //to get the last prefab in the file 
}

}//ns