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

    template<u8 QUAD_COUNT_X, u8 QUAD_COUNT_Z> 
    auto CreateMeshGrid(const f32 gridW, const f32 gridH)
    {
        box::SimpleArray<utils::Common_Vertex, QUAD_COUNT_Z * QUAD_COUNT_X * 6> arr;

        const auto quadW = gridW / QUAD_COUNT_X;
        const auto quadH = gridH / QUAD_COUNT_Z;

        for(u8 z = 0; z < QUAD_COUNT_Z; ++z) {
        for(u8 x = 0; x < QUAD_COUNT_X; ++x) { 
            const auto idx = (z * QUAD_COUNT_X + x) * 6;
            constexpr math::Vec4f col = { 0.1f, 0.7f, 0.1f, 1 };
            arr[idx + 0] = { {  0.0f + x, 0,  0.0f + z, 1 }, {}, col, {} };
            arr[idx + 1] = { {  1.0f + x, 0,  0.0f + z, 1 }, {}, col, {} };
            arr[idx + 2] = { {  1.0f + x, 0,  1.0f + z, 1 }, {}, col, {} };
            arr[idx + 3] = { {  0.0f + x, 0,  0.0f + z, 1 }, {}, col, {} };
            arr[idx + 4] = { {  1.0f + x, 0,  1.0f + z, 1 }, {}, col, {} };
            arr[idx + 5] = { {  0.0f + x, 0,  1.0f + z, 1 }, {}, col, {} };
        }}

        return arr;
    }

} //ns