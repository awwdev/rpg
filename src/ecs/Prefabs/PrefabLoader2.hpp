//https://github.com/awwdev

#pragma once
#include "ecs/ComponentsMeta/ComponentEnum.hpp"
#include "ecs/ComponentsMeta/ComponentData.hpp"
#include "ecs/Prefabs/PrefabEnum.hpp"
#include "res/Models/MeshEnum.hpp"
#include "dbg/Assert.hpp"
	
namespace rpg::ecs2 {

inline void LoadPrefabs(chars_t path)
{
    std::ifstream file(path);
    dbg::Assert(file.is_open(), "cannot open file");

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
            PrefabChild,
            Component,
            ComponentData,
            Invalid
        };

        //prefab language signifiers 
        const auto lineEnum = [&line] {
            switch(line[0]) {
                case '#': return LineEnum::Comment;       
                case '@': return LineEnum::Prefab;    
                case '&': return LineEnum::PrefabChild; 
                case '?': return LineEnum::Component; 
                case '+': return LineEnum::ComponentData; 
                default : return LineEnum::Invalid; 
            }
        }();

        //remove signifier
        std::memmove(&line[0], &line[1], lineLength);

        if (lineEnum == LineEnum::Invalid){
            dbg::LogWarning("bad parsing");
            continue;
        }

        //skip comments
        if (lineEnum == LineEnum::Comment)
            continue;

        //dbg::LogInfo(line);

        if (lineEnum == LineEnum::Prefab)   
        {
            if (auto const prefabEnum = PREFAB_STR_TO_ENUM.GetOptional(line)){
                dbg::LogInfo("Prefab", (int)*prefabEnum);
            }
            continue;
        } 


        if (lineEnum == LineEnum::PrefabChild)   
        {
            if (auto const prefabEnum = PREFAB_STR_TO_ENUM.GetOptional(line)){
                dbg::LogInfo("PrefabChild", (int)*prefabEnum);
            }
            continue;
        } 


        if (lineEnum == LineEnum::Component)   
        {
            if (auto const componentEnum = ecs::COMPONENT_STR_TO_ENUM.GetOptional(line)){
                dbg::LogInfo("Component", (int)*componentEnum);
            }
            continue;
        } 


        if (lineEnum == LineEnum::ComponentData)   
        {
            auto pair = ConvertToComponentDataPair(line);
            //const auto pair = ecs::ConvertToComponentDataStringPair(line);
            //TODO: split component key value
            //if (auto const componentDataEnum = ecs::COMPONENTDATA_STR_TO_ENUM.GetOptional(line)){
            //    dbg::LogInfo("ComponentData", (int)*componentDataEnum);
            //}
            //continue;
        } 
        
    }

}

}//ns