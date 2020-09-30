//https://github.com/awwdev

#pragma once
#include "ecs/EntityID.hpp"
#include "ecs/Prefabs/PrefabTypes.hpp"
#include "res/Models/MeshEnum.hpp"
#include "dbg/Assert.hpp"

#include <fstream>
#include <cctype>
	
namespace rpg::res2 {

constexpr idx_t PREFAB_FILE_LINE_LEN_MAX = 100;

inline void LoadPrefabs(chars_t path)
{
    std::ifstream file(path);
    dbg::Assert(file.is_open(), "cannot open file");

    //? LINE PARSING
    char line [PREFAB_FILE_LINE_LEN_MAX + 1] {}; //last will always be '\0'
    while(file.getline(line, PREFAB_FILE_LINE_LEN_MAX))  
    {
        //clean-up line
        auto const lineLength = std::strlen(line);
        for(std::size_t i = 0; line[i] != '\0';) {
            if (std::isgraph(line[i]) == false) {
                std::memmove(&line[i], &line[i + 1], lineLength - i);
                //continuous swapping might be better
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
            Child,
            Component,
            Data,
            Invalid
        };

        const auto lineEnum = [&line] {
            switch(line[0]) {
                case '#': return LineEnum::Comment;       
                case '@': return LineEnum::Prefab;    
                case '&': return LineEnum::Child; 
                case '?': return LineEnum::Component; 
                case '+': return LineEnum::Data; 
                default : return LineEnum::Invalid; 
            }
        }();

        //skip comments
        if (lineEnum == LineEnum::Comment)
            continue;

        dbg::LogInfo(line);

        
        
    }

}

}//ns