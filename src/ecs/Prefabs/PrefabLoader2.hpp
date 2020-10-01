//https://github.com/awwdev

#pragma once
#include "ecs/ComponentsMeta/ComponentEnum.hpp"
#include "ecs/ComponentsMeta/ComponentData.hpp"
#include "ecs/Prefabs/PrefabEnum.hpp"
#include "com/mem/Allocator.hpp"
#include "dbg/Assert.hpp"

namespace rpg::ecs2 {

inline void LoadPrefabs(chars_t path)
{
    std::ifstream file(path);
    dbg::Assert(file.is_open(), "cannot open file");

    //store all component data values per prefab
    constexpr auto COMPONENT_DATA_PAIR_MAX = 10;
    struct Arr { com::Array<ComponentDataPair, COMPONENT_DATA_PAIR_MAX> data [(idx_t) ecs::ComponentEnum::ENUM_END]; };
    auto  ptrComponentDataPairs = com::mem::ClaimBlock<Arr>();
    auto& componentDataPairs = ptrComponentDataPairs->data;

    constexpr auto NO_CURRENT_PREFAB    = PrefabEnum::ENUM_END;
    constexpr auto NO_CURRENT_COMPONENT = ecs::ComponentEnum::ENUM_END;
    auto currentPrefab    = NO_CURRENT_PREFAB;
    auto currentComponent = NO_CURRENT_COMPONENT;

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

            if (currentPrefab != NO_CURRENT_PREFAB) 
            { 
                //instantate previous prefab (data collection is done)

                //clear collected data of previous prefab
                FOR_CARRAY(componentDataPairs, i)
                    componentDataPairs[i].Clear();
            }
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
            auto const pair = ConvertToComponentDataPair(line);
            componentDataPairs[(idx_t) currentComponent].Append(pair);
            continue;
        } 
        
    }

}

}//ns