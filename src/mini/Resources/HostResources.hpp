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
        box::IndexMap<res::MeshVertexView, res::MeshType::ENUM_END> meshvertexLookup {
            { res::MeshType::PrimitiveCube,      { utils::MESH_CUBE, ARRAY_COUNT(utils::MESH_CUBE) } },
            { res::MeshType::PrimitiveQuad,      { utils::MESH_QUAD, ARRAY_COUNT(utils::MESH_QUAD) } },
            { res::MeshType::PrimitiveTriangle,  { utils::MESH_TRIANGLE, ARRAY_COUNT(utils::MESH_TRIANGLE) } },
            //adding dynamic ones when they are loaded
        }; 

        //TODO: huge array of vertices and then views
        box::Array<utils::VertexDefault, 250> sword;

        void Load()
        {
            LoadModel(sword, "res/Models/sword.txt");
            meshvertexLookup.Set(res::MeshType::Sword, res::MeshVertexView{sword.dataPtr, sword.Count()});
        }
    };

    struct HostResources
    {   
        Textures textures;
        Models   models;

        HostResources() 
        {
            //! should all load before any GPU resources are used
            textures.Load();
            models.Load();
        }
    }; 

}//ns