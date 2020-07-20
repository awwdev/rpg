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
#include "mini/Rendering/RenderGraph.hpp"

namespace mini::res
{
    struct Textures
    {
        TextureArray<96, 16, 16, 1> monospaceFont; // added one texture that is all FF which can be used for blank quad () 

        void Load()
        {
            monospaceFont.LoadArray("res/TextureArray"); //RLE would be nice
        }
    };

    struct Models
    {
        box::IndexMap<res::MeshVertexView, res::MeshType::ENUM_END> vertexLookup
        {
            //hardcoded primitives
            { res::MeshType::PrimitiveCube,      { utils::MESH_CUBE, ARRAY_COUNT(utils::MESH_CUBE) } },
            { res::MeshType::PrimitiveQuad,      { utils::MESH_QUAD, ARRAY_COUNT(utils::MESH_QUAD) } },
            { res::MeshType::PrimitiveTriangle,  { utils::MESH_TRIANGLE, ARRAY_COUNT(utils::MESH_TRIANGLE) } },
        }; 

        //model data separated
        box::Array<utils::Common_Vertex, rendering::DEFAULT_VERTEX_MAX_COUNT> vertices;

        void Load()
        {
            LoadModel(vertices, "res/Models/sword.txt");
            vertexLookup.Set(res::MeshType::Sword, res::MeshVertexView{vertices.dataPtr, vertices.Count()});
        }
    };

    struct HostResources
    {   
        Textures textures;
        Models   models;

        //! should all load before any GPU resources are used
        HostResources() 
        {
            textures.Load();
            models.Load();
        }
    }; 

}//ns