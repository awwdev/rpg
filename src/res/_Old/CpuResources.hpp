//https://github.com/awwdev
#pragma once

#include "com/box/Array.hpp"
#include "com/box/EnumMap.hpp"
#include "dbg/Logger.hpp"
#include "res/_Old/TextureLoader.hpp"
#include "res/_Old/TextureArray.hpp"
//#include "res/_Old/ModelLoader.hpp"
#include "res/_Old/CpuResources.hpp"
#include "res/Terrain/Terrain.hpp"
#include "gpu/RenderData/RenderData.hpp"
#include "gpu/RenderData/_Old/RenderStructs.hpp"
#include "res/Models/ModelPrimitives.hpp"

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
    //struct MeshVertexView
    //{
    //    const gpu::RenderData_General::Vertex* begin;
    //    idx_t count;
    //};

    //com::EnumMap<res::ModelType::ENUM_END, MeshVertexView> vertexLookup
    //{
    //    //hardcoded primitives
    //    { res::ModelType::PrimitiveCube,      { res::MESH_CUBE,     ArrayCount(res::MESH_CUBE)       } },
    //    { res::ModelType::PrimitiveQuad,      { res::MESH_QUAD,     ArrayCount(res::MESH_QUAD)       } },
    //    { res::ModelType::PrimitiveTriangle,  { res::MESH_TRIANGLE, ArrayCount(res::MESH_TRIANGLE)   } },
    //    { res::ModelType::PrimitiveRing16,    { res::MESH_RING_16,  ArrayCount(res::MESH_RING_16)     } },
    //}; 

    //com::Array<gpu::RenderData_General::Vertex, gpu::DEFAULT_VERTEX_MAX_COUNT> allModelVertices;

    void Load()
    {
        //com::Array<gpu::RenderData_General::Vertex, gpu::DEFAULT_VERTEX_MAX_COUNT> tmp;

        //const auto LoadModelFn = [&](chars_t path, const res::ModelType type){
        //    LoadModelOld(tmp, path);
        //    const auto prevCount = allModelVertices.Count();
        //    allModelVertices.AppendArray(tmp);
        //    vertexLookup.Set(type, MeshVertexView{ &allModelVertices[prevCount], tmp.Count() });
        //    tmp.Clear();
        //};

        //LoadModelFn("res/Models/sword.txt", res::ModelType::Sword);
        //LoadModelFn("res/Models/grass.txt", res::ModelType::Grass);
        //LoadModelFn("res/Models/stone.txt", res::ModelType::Stone);
        //LoadModelFn("res/Models/tree.txt",  res::ModelType::Tree);
    }
};

struct CpuResources
{   
    Textures textures;
    //Models   models;

    res::Terrain<10, 10, 2> terrain; //!move it out

    //! should all load before any GPU resources are used
    CpuResources() 
    {
        textures.Load();
        //models.Load();
        terrain.Create();
    }
}; 

}//ns