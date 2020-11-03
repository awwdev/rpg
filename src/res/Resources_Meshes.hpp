//https://github.com/awwdev

#pragma once
#include "res/Mesh/MeshMeta.hpp"
#include "res/Mesh/MeshLoader.hpp"

#include "com/box/EnumMap.hpp"
#include "com/utils/Utils.hpp"

namespace rpg::res {

struct Resources_Meshes
{
    VertexArray  allVertices;
    IndexArray   allIndices;
    VertexRanges vertexRanges;
    IndexRanges  indexRanges;

    void Load()
    {
        for (MeshEnum meshEnum { (MeshEnum) 1 };;) //don't load MeshNone
        {
            const auto meshIdx = static_cast<idx_t>(meshEnum);
            dbg::Assert(MESH_PATHS.Contains(meshIdx), "mesh missing");
            auto const path = MESH_PATHS.Get(meshIdx).Data();
            LoadMesh(path,  meshIdx,  allVertices, allIndices, vertexRanges, indexRanges);
            if (com::AdvanceEnum(meshEnum))
                break;
        }
    }
};

}//ns