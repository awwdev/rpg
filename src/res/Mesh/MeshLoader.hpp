//https://github.com/awwdev

#pragma once
#include "gpu/RenderData/RenderData_General.hpp"
#include "res/Mesh/MeshMeta.hpp"
#include "com/box/Array.hpp"

#include <fstream>

namespace rpg::res {

inline auto LoadMesh(chars_t path, idx_t const meshIdx,
VertexArray& allVertices, IndexArray& allIndices, VertexRanges& vertexRanges)
{
    //file
    std::ifstream file(path, std::ios::binary);
    dbg::Assert(file.is_open(), "cannot open file");

    //mode
    enum class DataEnum 
    {
        None,
        IndexData,
        VertexData,
        FaceNormalData,
        FaceColorData,
    }
    dataEnum {};

    //lines meta
    constexpr auto LINE_LEN_MAX = 150;
    char line [LINE_LEN_MAX];
    const auto vertexRangeIndex = allVertices.Count(); //begin

    //parsing functions
    auto const parseIndex = [&](auto const data)
    {
        auto const index = static_cast<uint32_t>(data);
        allIndices.AppendElement(index);
        dbg::LogInfo(index);
    };

    //lines
    while(file.getline(line, LINE_LEN_MAX))
    {
        //current data mode
        switch (line[0])
        {
            case 'I': dataEnum = DataEnum::IndexData;       continue;
            case 'V': dataEnum = DataEnum::VertexData;      continue;
            case 'F': dataEnum = DataEnum::FaceNormalData;  continue;
            case 'C': dataEnum = DataEnum::FaceColorData;   continue;
            default: break;
        }

        //line parsing
        idx_t begin = 0;
        idx_t commaCount = 0;
        for(idx_t i = 0; line[i] != '\n' && line[i] != '\0'; ++i)
        {
            if (line[i] != ',')
            continue;

            //data parsing
            const f32 value = std::atof(&line[begin]);
            switch(dataEnum)
            {
                case DataEnum::IndexData:       parseIndex(value); break;
                case DataEnum::VertexData:      break;
                case DataEnum::FaceNormalData:  break;
                case DataEnum::FaceColorData:   break;
                case DataEnum::None:            break; //! have to be error later
                default: dbg::Assert(false, "parsing mode missing"); break;
            };

            //advance
            begin = i + 1;
            commaCount++;   
        }

    }

}//ns

/*
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
        case  4: vertex.col.r = value; break;
        case  5: vertex.col.g = value; break;
        case  6: vertex.col.b = value; break;
        case  7: vertex.col.a = value; break;
        //vert nor
        case  8: vertex.nor.x = value; break;
        case  9: vertex.nor.y = value; break;
        case 10: vertex.nor.z = value; break;
        //vert tex
        case 11: vertex.tex.x = value; break;
        case 12: vertex.tex.y = value; break;
    } 
    */
    //begin = i + 1;
    //commaCount++;    
//}

// allVertices.AppendElement(vertex);
//}

//const auto vertexRangeCount = allVertices.Count() - vertexRangeIndex;
//vertexRanges[meshIdx] = { (uint32_t) vertexRangeIndex, (uint32_t) vertexRangeCount };

}//ns