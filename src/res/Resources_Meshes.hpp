//https://github.com/awwdev

#pragma once
#include "res/Mesh/MeshMeta.hpp"
#include "res/Mesh/MeshLoader.hpp"

#include "com/box/EnumMap.hpp"

namespace rpg::res {

struct Resources_Meshes
{
    com::Array<MeshVertex, MESHES_VERTS_TOTAL> allVertices;
    MeshVertexRange meshVertexRanges [MESHES_TOTAL];

    void Load()
    {
        FOR_ARRAY(MESH_PATHS.usedIndices, i) {
            auto const idx  = MESH_PATHS.usedIndices[i];
            auto const eIdx = (MeshEnum) idx;
            auto const path = MESH_PATHS.Get(idx).cstr;
            LoadMesh(path,  eIdx,  allVertices, meshVertexRanges);
        }
    }
};

}//ns