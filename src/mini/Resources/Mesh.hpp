//https://github.com/awwdev

#pragma once
#include "mini/Math/Matrix.hpp"
#include "mini/Utils/PrimitiveMeshes.hpp"
#include "mini/box/IndexMap.hpp"
#include "mini/Memory/Allocator.hpp"

namespace mini::resources
{
    enum MeshType
    {
        Sword,
        PrimitiveCube,
        PrimitiveQuad,
        PrimitiveTriangle,
        ENUM_END
    };

    struct MeshVertexView
    {
        const utils::Vertex* begin;
        u32 count;
    };

    //the actual vertex data is stored somehwere else
    box::IndexMap<MeshVertexView, MeshType::ENUM_END> MESH_VERTEX_MAP {
        { MeshType::PrimitiveCube,      { utils::MESH_CUBE, ARRAY_COUNT(utils::MESH_CUBE) } },
        { MeshType::PrimitiveQuad,      { utils::MESH_QUAD, ARRAY_COUNT(utils::MESH_QUAD) } },
        { MeshType::PrimitiveTriangle,  { utils::MESH_TRIANGLE, ARRAY_COUNT(utils::MESH_TRIANGLE) } },
        //adding dynamic ones when they are loaded
    }; 

} //ns