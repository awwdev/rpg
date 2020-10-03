//https://github.com/awwdev

#pragma once
#include "gpu/RenderData/RenderData_General.hpp"
#include "res/Mesh/MeshMeta.hpp"
#include "com/box/Array.hpp"

#include <fstream>

namespace rpg::res {

inline auto LoadMesh(chars_t path, MeshEnum const& meshEnum,
com::Array<MeshVertex, MESHES_VERTS_TOTAL>& allVertices,
MeshVertexRange (&meshVertexRanges) [MESHES_TOTAL])
{
    std::ifstream file(path, std::ios::binary);
    dbg::Assert(file.is_open(), "cannot open file");

    constexpr auto LINE_LEN_MAX = 150;
    char line [LINE_LEN_MAX];

    const auto vertexRangeIndex = allVertices.Count(); //begin

    while(file.getline(line, LINE_LEN_MAX))
    {
        MeshVertex vertex {};

        idx_t begin = 0;
        idx_t commaCount = 0;

        for(idx_t i = 0; line[i] != '\n' && line[i] != '\0'; ++i)
        {
            if (line[i] != ',')
                continue;

            const f32 value = (f32)std::atof(&line[begin]);

            constexpr auto COMMAS_MAX = 13;
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

            begin = i + 1;
            commaCount++;    
        }

        allVertices.Append(vertex);
    }

    const auto vertexRangeCount = allVertices.Count() - vertexRangeIndex;
    meshVertexRanges[(idx_t) meshEnum] = { (uint32_t) vertexRangeIndex, (uint32_t) vertexRangeCount };
}

}//ns