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

    //?parsing functions
    auto const parseIndex = [&](auto const data)
    {
        auto const index = static_cast<uint32_t>(data);
        allIndices.AppendElement(index);
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
            case CommaEnum::Col_A: vertex.col.a = data; 
            //vert tex
            //case CommaEnum::Tex_U: vertex.tex.x = data; break;
            //case CommaEnum::Tex_V: vertex.tex.y = data; break;
                allVertices.AppendElement(vertex); 
                com::PrintMatrix(vertex.pos);
                com::PrintMatrix(vertex.nor);
                com::PrintMatrix(vertex.col);
            break;
            case CommaEnum::VERTEX_FINISHED: dbg::Assert(false, "enum missing"); break;
        } 

    };

    auto const parseFaceNormal = [&](auto const data)
    {
    };

    auto const parseFaceColor = [&](auto const data)
    {
    };

    //lines
    while(file.getline(line, LINE_LEN_MAX))
    {
        idx_t begin = 0;
        idx_t commaCount = 0;

        //current data mode
        switch (line[0])
        {
            case 'I': dataEnum = DataEnum::IndexData;       commaCount = 0; continue;
            case 'V': dataEnum = DataEnum::VertexData;      commaCount = 0; continue;
            case 'F': dataEnum = DataEnum::FaceNormalData;  commaCount = 0; continue;
            case 'C': dataEnum = DataEnum::FaceColorData;   commaCount = 0; continue;
            default: break;
        }

        //line parsing
        for(idx_t i = 0; line[i] != '\n' && line[i] != '\0'; ++i)
        {
            if (line[i] != ',')
            continue;

            //data parsing
            const f32 value = std::atof(&line[begin]);
            switch(dataEnum)
            {
                case DataEnum::IndexData:       parseIndex(value); break;
                case DataEnum::VertexData:      parseVertex(value, commaCount); break;
                case DataEnum::FaceNormalData:  parseFaceNormal(value); break;
                case DataEnum::FaceColorData:   parseFaceColor(value); break;
                case DataEnum::None:            break; //! have to be error later
                default: dbg::Assert(false, "parsing mode missing"); break;
            };

            //advance
            begin = i + 1;
            commaCount++;   
        }

    }

}//ns

//TODO: rendering before doing any other data

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