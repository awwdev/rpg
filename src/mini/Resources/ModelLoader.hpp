//https://github.com/awwdev
#pragma once

#include "mini/Utils/Types.hpp"
#include "mini/Utils/Structs.hpp"
#include <fstream>
#include <filesystem>

namespace mini::res {

template<auto N>
void LoadModel(box::Array<utils::Common_Vertex, N>& vertices, chars_t path)
{
    using namespace utils;
    
    std::ifstream file(path, std::ios::binary);
    if (!file) dbg::LogError("cannot open file");

    utils::Common_Vertex vertex {};

    constexpr auto BUFFER_MAX = 150;
    char line [BUFFER_MAX];
    while(file.getline(line, BUFFER_MAX))
    {
        idx_t beg = 0;
        idx_t commaCount = 0;

        auto parseFn = [&] {
            f32 value = std::atof(&line[beg]);
            switch(commaCount % 13){
                //vert index
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
                //vert normals
                case  8: vertex.nor[X] = value; break;
                case  9: vertex.nor[Y] = value; break;
                case 10: vertex.nor[Z] = value; break;
                //vert uv
                case 11: vertex.tex[X] = value; break;
                case 12: vertex.tex[Y] = value; break;
            }           
        };

        for(idx_t i = 0; line[i] != '\n' && line[i] != '\0'; ++i)
        {
            const auto c = line[i];
            if (c == ','){ 
                parseFn();
                beg = i + 1;
                commaCount++;

                //if (commaCount % 13 == 0)
                //    dbg::LogInfo(vertex);
            }
        }
    }

}

}//NS