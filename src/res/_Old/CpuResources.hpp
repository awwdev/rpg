//https://github.com/awwdev
#pragma once

#include "com/Containers/Array.hpp"
#include "com/Containers/EnumMap.hpp"
#include "dbg/Logger.hpp"
#include "res/_Old/TextureLoader.hpp"
#include "res/_Old/TextureArray.hpp"
#include "res/_Old/ModelLoader.hpp"
#include "res/_Old/CpuResources.hpp"
#include "res/Terrain/Terrain.hpp"
#include "gpu/RenderData/RenderData.hpp"
#include "gpu/RenderData/_Old/RenderStructs.hpp"
#include "com/PrimitiveMeshes.hpp"

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
        const gpu::RenderData_General::Vertex* begin;
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

    com::Array<gpu::RenderData_General::Vertex, gpu::DEFAULT_VERTEX_MAX_COUNT> allModelVertices;

    void Load()
    {
        com::Array<gpu::RenderData_General::Vertex, gpu::DEFAULT_VERTEX_MAX_COUNT> tmp;

        const auto LoadModelFn = [&](chars_t path, const res::MeshType type){
            LoadModel(tmp, path);
            const auto prevCount = allModelVertices.Count();
            allModelVertices.AppendArray(tmp);
            vertexLookup.Set(type, MeshVertexView{ &allModelVertices[prevCount], tmp.Count() });
            tmp.Clear();
        };

        LoadModelFn("res/Models/sword.txt", res::MeshType::Sword);
        LoadModelFn("res/Models/grass.txt", res::MeshType::Grass);
        LoadModelFn("res/Models/stone.txt", res::MeshType::Stone);
        LoadModelFn("res/Models/tree.txt",  res::MeshType::Tree);
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