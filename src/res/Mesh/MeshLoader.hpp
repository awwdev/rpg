//https://github.com/awwdev

#pragma once
#include "gpu/RenderData/RenderData_Main.hpp"
#include "res/Mesh/MeshMeta.hpp"
#include "com/box/Array.hpp"

#include <fstream>

namespace rpg::res {

inline auto LoadMesh(chars_t path, idx_t const meshIdx,
VertexArray& allVertices, IndexArray& allIndices, 
VertexRanges& vertexRanges, IndexRanges& indexRanges)
{
    //file
    ::std::ifstream file(path, ::std::ios::binary);
    dbg::Assert(file.is_open(), "cannot open file");

    //mode
    enum class DataEnum 
    {
        None,
        IndexData,
        VertexData,
        //FaceNormalData,
        //FaceColorData,
    }
    dataEnum {};

    //lines meta
    constexpr auto LINE_LEN_MAX = 150;
    char line [LINE_LEN_MAX];
    const auto vertexRangeIndex = allVertices.Count(); //begin

    //?parsing functions
    uint32_t indexCount = 0;
    auto const parseIndex = [&](auto const data)
    {
        auto const index = static_cast<uint32_t>(data);
        allIndices.AppendElement(index);
        ++indexCount;
    };

    MeshVertex vertex {};
    auto const parseVertex = [&](auto const data, idx_t const commaCount)
    {
        enum struct CommaEnum : idx_t
        {
            Index,
            Pos_X, Pos_Y, Pos_Z, 
            Nor_X, Nor_Y, Nor_Z,
            Col_R, Col_G, Col_B, Col_A,
            //Tex_U, Tex_V,
            VERTEX_FINISHED,
        };
 
        switch((CommaEnum)(commaCount % (idx_t) CommaEnum::VERTEX_FINISHED))
        {
            //vert idx
            case CommaEnum::Index: break;
            //vert pos
            case CommaEnum::Pos_X: vertex.pos.x = data; break;
            case CommaEnum::Pos_Y: vertex.pos.y = data; break;
            case CommaEnum::Pos_Z: vertex.pos.z = data; break;
            //vert nor
            case CommaEnum::Nor_X: vertex.nor.x = data; break;
            case CommaEnum::Nor_Y: vertex.nor.y = data; break;
            case CommaEnum::Nor_Z: vertex.nor.z = data; break;
            //vert col
            case CommaEnum::Col_R: vertex.col.r = data; break;
            case CommaEnum::Col_G: vertex.col.g = data; break;
            case CommaEnum::Col_B: vertex.col.b = data; break;
            case CommaEnum::Col_A: vertex.col.a = data; //break;
            //vert tex
            //case CommaEnum::Tex_U: vertex.tex.x = data; break;
            //case CommaEnum::Tex_V: vertex.tex.y = data; 
            {
                allVertices.AppendElement(vertex); 
                const auto vertexRangeCount = allVertices.Count() - vertexRangeIndex;
                vertexRanges[meshIdx] = { (uint32_t) vertexRangeIndex, (uint32_t) vertexRangeCount };
            }
            break;
            case CommaEnum::VERTEX_FINISHED: dbg::Assert(false, "enum missing"); break;
        } 

    };

    //lines
    while(file.getline(line, LINE_LEN_MAX))
    {
        idx_t begin = 0;
        idx_t commaCount = 0;

        //current data mode
        switch (line[0])
        {
            case 'I': dataEnum = DataEnum::IndexData;
            indexCount = 0;       
            indexRanges[meshIdx].index = allIndices.Count();
            commaCount = 0; 
            continue;

            case 'V': dataEnum = DataEnum::VertexData;      
            indexRanges[meshIdx].count = indexCount;
            commaCount = 0; 
            continue;

            default: break;
        }

        //line parsing
        for(idx_t i = 0; line[i] != '\n' && line[i] != '\0'; ++i)
        {
            if (line[i] != ',')
            continue;

            //data parsing
            const f32 value = ::std::atof(&line[begin]);
            switch(dataEnum)
            {
                case DataEnum::IndexData:       parseIndex(value); break;
                case DataEnum::VertexData:      parseVertex(value, commaCount); break;
                case DataEnum::None:            break; //! have to be error later
                default: dbg::Assert(false, "parsing mode missing"); break;
            };

            //advance
            begin = i + 1;
            commaCount++;   
        }
    }

}//fn

}//ns