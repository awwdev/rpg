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
        for(idx_t meshIdx = (idx_t) MeshEnum::None + 1; meshIdx < (idx_t) MeshEnum::ENUM_END; ++meshIdx)
        {
            dbg::Assert(MESH_PATHS.Contains(meshIdx), "mesh missing");
            auto const path = MESH_PATHS.Get(meshIdx).Data();
            LoadMesh(path,  meshIdx,  allVertices, meshVertexRanges);
        }
    }
};

}//ns