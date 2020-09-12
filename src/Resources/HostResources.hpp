//https://github.com/awwdev
#pragma once

#include "Common/Container/Array.hpp"
#include "Common/Container/EnumMap.hpp"
#include "Debug/Logger.hpp"
#include "Resources/TextureLoader.hpp"
#include "Resources/TextureArray.hpp"
#include "Resources/ModelLoader.hpp"
#include "Resources/MeshTypes.hpp"
#include "Resources/Terrain/Terrain.hpp"
#include "GPU/RenderData.hpp"
#include "Common/PrimitiveMeshes.hpp"

namespace rpg::res {
    
struct Textures
{
    TextureArray<128, 16, 16, 1> monospaceFont;
    TextureArray<2, 512, 512, 1> common;

    void Load()
    {
        monospaceFont.LoadArray("res/TextureArrays/MonospaceFont"); //RLE would be nice
        common.LoadArray("res/TextureArrays/Default");
    }
};

struct Models
{
    struct MeshVertexView
    {
        const use::Common_Vertex* begin;
        idx_t count;
    };

    com::EnumMap<res::MeshType::ENUM_END, MeshVertexView> vertexLookup
    {
        //hardcoded primitives
        { res::MeshType::PrimitiveCube,      { use::MESH_CUBE,     ArrayCount(use::MESH_CUBE)       } },
        { res::MeshType::PrimitiveQuad,      { use::MESH_QUAD,     ArrayCount(use::MESH_QUAD)       } },
        { res::MeshType::PrimitiveTriangle,  { use::MESH_TRIANGLE, ArrayCount(use::MESH_TRIANGLE)   } },
        { res::MeshType::PrimitiveRing16,    { use::MESH_RING_16,  ArrayCount(use::MESH_RING_16)     } },
    }; 

    com::Array<use::Common_Vertex, gpu::DEFAULT_VERTEX_MAX_COUNT> allModelVertices;

    void Load()
    {
        com::Array<use::Common_Vertex, gpu::DEFAULT_VERTEX_MAX_COUNT> tmp;

        const auto LoadModelFn = [&](chars_t path, const res::MeshType type){
            LoadModel(tmp, path);
            vertexLookup.Set(type, MeshVertexView{ &allModelVertices[allModelVertices.count], tmp.count });
            allModelVertices.AppendArray(tmp);
            tmp.Clear();
        };

        LoadModelFn("res/Models/sword.txt", res::MeshType::Sword);
        LoadModelFn("res/Models/grass.txt", res::MeshType::Grass);
        LoadModelFn("res/Models/stone.txt", res::MeshType::Stone);
        LoadModelFn("res/Models/tree.txt",  res::MeshType::Tree);
    }
};

struct HostResources
{   
    Textures textures;
    Models   models;

    res::Terrain<10, 10, 2> terrain;

    //! should all load before any GPU resources are used
    HostResources() 
    {
        textures.Load();
        models.Load();
        terrain.Create();
    }
}; 

}//ns