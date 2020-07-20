//https://github.com/awwdev

#pragma once
#include "mini/Math/Matrix.hpp"
#include "mini/Utils/PrimitiveMeshes.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/box/IndexMap.hpp"
#include "mini/Memory/Allocator.hpp"

namespace mini::res
{
    enum class MeshType
    {
        PrimitiveCube,
        PrimitiveQuad,
        PrimitiveTriangle,
        //TerrainStatic,
        Sword,
        ENUM_END
    };

    struct MeshVertexView
    {
        const utils::Common_Vertex* begin;
        u32 count;
    };

    template<u32 N>
    box::SimpleArray<utils::Common_Vertex, N> CreateMeshGrid(
        const f32 width, 
        const f32 heigth, 
        const u8  cellCountW, 
        const u8  cellCountH)
    {
        return {};
    }

} //ns