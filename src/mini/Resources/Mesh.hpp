//https://github.com/awwdev

#pragma once
#include "mini/Math/Matrix.hpp"
#include "mini/Utils/PrimitiveMeshes.hpp"
#include "mini/box/IndexMap.hpp"
#include "mini/Memory/Allocator.hpp"

namespace mini::res
{
    enum MeshType
    {
        PrimitiveCube,
        PrimitiveQuad,
        PrimitiveTriangle,
        Sword,
        ENUM_END
    };

    struct MeshVertexView
    {
        const utils::VertexDefault* begin;
        u32 count;
    };

} //ns