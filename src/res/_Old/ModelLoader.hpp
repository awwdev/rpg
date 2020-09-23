//https://github.com/awwdev
#pragma once

#include "com/Types.hpp"
#include "com/Structs.hpp"
#include "gpu/RenderData/_Old/RenderStructs.hpp"
#include <fstream>
#include <filesystem>

namespace rpg::res {

template<auto N>
void LoadModel(com::Array<gpu::RenderData_General::Vertex, N>& vertices, chars_t path)
{
    using namespace com;
    
    std::ifstream file(path, std::ios::binary);
    if (!file) dbg::LogError("cannot open file");

    constexpr auto BUFFER_MAX = 150;
    constexpr auto COMMAS_MAX = 13;

    char line [BUFFER_MAX];
    gpu::RenderData_General::Vertex vertex {};

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
                case  1: vertex.pos.x = value; break;
                case  2: vertex.pos.y = value; break;
                case  3: vertex.pos.z = value; break;
                //vert col
                case  4: vertex.col.x = value; break;
                case  5: vertex.col.y = value; break;
                case  6: vertex.col.z = value; break;
                case  7: vertex.col.w = value; break;
                //vert nor
                case  8: vertex.nor.x = value; break;
                case  9: vertex.nor.y = value; break;
                case 10: vertex.nor.z = value; break;
                //vert tex
                case 11: vertex.tex.x = value; break;
                case 12: vertex.tex.y = value; break;
            } 

            beg = i + 1;
            commaCount++;    
        }

        //dbg::LogInfo(vertex);
        vertices.Append(vertex);
        
    }

}

}//NS