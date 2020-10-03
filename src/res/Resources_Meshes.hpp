//https://github.com/awwdev

#pragma once
#include "res/Mesh/MeshMeta.hpp"
#include "res/Mesh/MeshLoader.hpp"

namespace rpg::res {

struct Resources_Meshes
{
    com::Array<MeshVertex, MESHES_VERTS_ALL> allVertices;
    MeshVertexRange meshVertexRanges [MESHES_TOTAL];

    void Load()
    {
        LoadMesh("res/Meshes/Sword_Grib.mesh",  MeshEnum::Sword_Grib,  allVertices, meshVertexRanges);
        LoadMesh("res/Meshes/Sword_Blade.mesh", MeshEnum::Sword_Blade, allVertices, meshVertexRanges);
    }
};

}//ns