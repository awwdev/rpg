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

    template<u8 QUAD_COUNT_W, u8 QUAD_COUNT_H> 
    auto CreateMeshGrid(const f32 gridW, const f32 gridH)
    {
        box::SimpleArray<utils::Common_Vertex, QUAD_COUNT_W * QUAD_COUNT_H * 6> arr;

        const auto quadW = gridW / QUAD_COUNT_W;
        const auto quadH = gridH / QUAD_COUNT_H;

        for(u8 y = 0; y < QUAD_COUNT_H; ++y) {
        for(u8 x = 0; x < QUAD_COUNT_W; ++x) { 
            const auto idx = (y * QUAD_COUNT_W + x) * 6;
            constexpr math::Vec4f col = { 0.1f, 0.7f, 0.1f, 1 };
            arr[idx + 0] = { { -1.0f, 0, -1.0f, 1 }, {}, col, {} };
            arr[idx + 1] = { {  1.0f, 0, -1.0f, 1 }, {}, col, {} };
            arr[idx + 2] = { {  1.0f, 0,  1.0f, 1 }, {}, col, {} };
            arr[idx + 3] = { { -1.0f, 0, -1.0f, 1 }, {}, col, {} };
            arr[idx + 4] = { {  1.0f, 0,  1.0f, 1 }, {}, col, {} };
            arr[idx + 5] = { { -1.0f, 0,  1.0f, 1 }, {}, col, {} };
        }}

        return arr;
    }

} //ns