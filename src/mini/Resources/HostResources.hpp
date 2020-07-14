//https://github.com/awwdev
#pragma once

#include "mini/Memory/Allocator.hpp"
#include "mini/Resources/TextureLoader.hpp"
#include "mini/Box/Map.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Resources/TextureArray.hpp"
#include "mini/Resources/ModelLoader.hpp"
#include "mini/Resources/Mesh.hpp"

namespace mini::hostRes
{
    struct HostResources
    {
        TextureArray<96, 16, 16, 1> fontTextures; // added one texture that is all FF which can be used for blank quad () 

        box::Array<utils::Vertex, 250> sword;

        HostResources() 
        {
            //currently load is instant inside ctor
            fontTextures.LoadArray("res/TextureArray"); //RLE would be nice

            LoadModel(sword, "res/Models/sword.txt");
            resources::MESH_VERTEX_MAP.Set(resources::MeshType::Sword, resources::MeshVertexView{sword.dataPtr, sword.Count()});
        }
    }; 

}//ns