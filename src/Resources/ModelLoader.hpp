//https://github.com/awwdev
#pragma once

#include "Utils/Types.hpp"
#include "Utils/Structs.hpp"
#include <fstream>
#include <filesystem>

namespace rpg::res {

template<auto N>
void LoadModel(box::Array<utils::Common_Vertex, N>& vertices, chars_t path)
{
    using namespace utils;
    
    std::ifstream file(path, std::ios::binary);
    if (!file) dbg::LogError("cannot open file");

    constexpr auto BUFFER_MAX = 150;
    constexpr auto COMMAS_MAX = 13;

    char line [BUFFER_MAX];
    Common_Vertex vertex {};

    while(file.getline(line, BUFFER_MAX))
    {
        idx_t beg = 0;
        idx_t commaCount = 0;

        for(idx_t i = 0; line[i] != '\n' && line[i] != '\0'; ++i)
        {
            if (line[i] != ',')
                continue;

            const f32 value = (f32)std::atof(&line[beg]);
            
            switch(commaCount % COMMAS_MAX)
            {
                //vert idx
                case  0: break;
                //vert pos
                case  1: vertex.pos[X] = value; break;
                case  2: vertex.pos[Y] = value; break;
                case  3: vertex.pos[Z] = value; break;
                //vert col
                case  4: vertex.col[X] = value; break;
                case  5: vertex.col[Y] = value; break;
                case  6: vertex.col[Z] = value; break;
                case  7: vertex.col[W] = value; break;
                //vert nor
                case  8: vertex.nor[X] = value; break;
                case  9: vertex.nor[Y] = value; break;
                case 10: vertex.nor[Z] = value; break;
                //vert tex
                case 11: vertex.tex[X] = value; break;
                case 12: vertex.tex[Y] = value; break;
            } 

            beg = i + 1;
            commaCount++;    
        }

        //dbg::LogInfo(vertex);
        vertices.Append(vertex);
        
    }

}

}//NS