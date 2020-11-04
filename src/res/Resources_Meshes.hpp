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
        for (idx_t meshIdx = 0; meshIdx < (idx_t) res::MeshEnum::ENUM_END; ++meshIdx)
        {
            dbg::Assert(MESH_PATHS.Contains(meshIdx), "mesh missing");
            auto const path = MESH_PATHS.Get(meshIdx).Data();
            LoadMesh(path,  meshIdx,  allVertices, allIndices, vertexRanges, indexRanges);
        }
    }
};

}//ns