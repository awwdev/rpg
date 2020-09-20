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
#include "GPU/VertexData.hpp"
#include "Common/PrimitiveMeshes.hpp"

namespace rpg::res {
    
struct Textures
{
    TextureArray<128, 16, 16, 1> monospaceFont;
    TextureArray<2, 512, 512, 1> common;

    void Load()
    {
        monospaceFont.LoadArray("AssetsApp/TextureArrays/MonospaceFont"); //RLE would be nice
        common.LoadArray("AssetsApp/TextureArrays/Default");
    }
};

struct Models
{
    struct MeshVertexView
    {
        const gpu::GeneralVertex* begin;
        idx_t count;
    };

    com::EnumMap<res::MeshType::ENUM_END, MeshVertexView> vertexLookup
    {
        //hardcoded primitives
        { res::MeshType::PrimitiveCube,      { com::MESH_CUBE,     ArrayCount(com::MESH_CUBE)       } },
        { res::MeshType::PrimitiveQuad,      { com::MESH_QUAD,     ArrayCount(com::MESH_QUAD)       } },
        { res::MeshType::PrimitiveTriangle,  { com::MESH_TRIANGLE, ArrayCount(com::MESH_TRIANGLE)   } },
        { res::MeshType::PrimitiveRing16,    { com::MESH_RING_16,  ArrayCount(com::MESH_RING_16)     } },
    }; 

    com::Array<gpu::GeneralVertex, gpu::DEFAULT_VERTEX_MAX_COUNT> allModelVertices;

    void Load()
    {
        com::Array<gpu::GeneralVertex, gpu::DEFAULT_VERTEX_MAX_COUNT> tmp;

        const auto LoadModelFn = [&](chars_t path, const res::MeshType type){
            LoadModel(tmp, path);
            vertexLookup.Set(type, MeshVertexView{ &allModelVertices[allModelVertices.count], tmp.count });
            allModelVertices.AppendArray(tmp);
            tmp.Clear();
        };

        LoadModelFn("AssetsApp/Models/sword.txt", res::MeshType::Sword);
        LoadModelFn("AssetsApp/Models/grass.txt", res::MeshType::Grass);
        LoadModelFn("AssetsApp/Models/stone.txt", res::MeshType::Stone);
        LoadModelFn("AssetsApp/Models/tree.txt",  res::MeshType::Tree);
    }
};

struct CpuResources
{   
    Textures textures;
    Models   models;

    res::Terrain<10, 10, 2> terrain; //!move it out

    //! should all load before any GPU resources are used
    CpuResources() 
    {
        textures.Load();
        models.Load();
        terrain.Create();
    }
}; 

}//ns