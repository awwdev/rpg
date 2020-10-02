//https://github.com/awwdev

#pragma once
#include "res/Meshes/MeshMeta.hpp"
#include "res/Meshes/MeshLoader.hpp"

namespace rpg::res {

struct Resources_Meshes
{
    com::Array<MeshVertex, MESHES_VERTS_ALL> allVertices;
    com::Array<MeshView,   MESHES_TOTAL>     meshViews;

    void Load()
    {
        LoadMesh("res/Meshes/Sword_Blade.mesh", allVertices, meshViews);
        LoadMesh("res/Meshes/Sword_Grib.mesh",  allVertices, meshViews);
    }
};

}//ns