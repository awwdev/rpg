//https://github.com/awwdev
#pragma once

#include "mini/Box/Array.hpp"
#include "mini/Box/EnumMap.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Resources/TextureLoader.hpp"
#include "mini/Resources/TextureArray.hpp"
#include "mini/Resources/ModelLoader.hpp"
#include "mini/Resources/Mesh.hpp"
#include "mini/Resources/Terrain/Terrain.hpp"
#include "mini/Rendering/RenderGraph.hpp"

namespace mini::res {
    
struct Textures
{
    TextureArray<128, 16, 16, 1> monospaceFont;

    void Load()
    {
        monospaceFont.LoadArray("res/TextureArray"); //RLE would be nice
    }
};

struct Models
{
    box::EnumMap<res::MeshType::ENUM_END, res::MeshVertexView> vertexLookup
    {
        //hardcoded primitives
        { res::MeshType::PrimitiveCube,      { utils::MESH_CUBE,     ArrayCount(utils::MESH_CUBE)       } },
        { res::MeshType::PrimitiveQuad,      { utils::MESH_QUAD,     ArrayCount(utils::MESH_QUAD)       } },
        { res::MeshType::PrimitiveTriangle,  { utils::MESH_TRIANGLE, ArrayCount(utils::MESH_TRIANGLE)   } },
        { res::MeshType::PrimitiveRing8,     { utils::MESH_RING_8,   ArrayCount(utils::MESH_RING_8)     } },
    }; 

    //model data separated
    box::Array<utils::Common_Vertex, rendering::DEFAULT_VERTEX_MAX_COUNT> vertices;

    void Load()
    {
        LoadModel(vertices, "res/Models/sword.txt");
        vertexLookup.Set(res::MeshType::Sword, res::MeshVertexView{(utils::Common_Vertex*)vertices.bytes, vertices.count});
    }
};

struct HostResources
{   
    Textures textures;
    Models   models;

    res::Terrain<20, 5, 2> terrain;

    //! should all load before any GPU resources are used
    HostResources() 
    {
        textures.Load();
        models.Load();
        terrain.Create();
    }
}; 

}//ns