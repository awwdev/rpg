#pragma once

#include "mini/Box/Array.hpp"
#include "mini/Box/String.hpp"
#include "mini/Box/Map.hpp"
#include "mini/Utils/Vertex.hpp"
#include "mini/Memory/Allocator.hpp"
#include "mini/Math/Matrix.hpp"

namespace mini::res
{
    using M = box::Map<math::Vec2f, 256>;
    using P = M::Pair_t;
    const M MAPPING
    {
        P { 'H', { 0, 2 } },
        P { 'E', { 1, 2 } },
        P { 'Y', { 3, 3 } },
    };  
    const math::Vec2f LETTER_SIZE { 14, 18 };


    struct Quad { Vertex vertices [6]; };

    inline Quad CreateQuad(
        const u32 x,  const u32 y, 
        const u32 w,  const u32 h,
        const u32 tx, const u32 ty)
    {
        const float _tx = tx / (float)w;
        const float _ty = 1.f - (ty / (float)h);
        const float _tw = _tx + 1 / (float)w;
        const float _th = 0;//_ty - 1 / (float)h;

        return {
            Vertex{ .pos { -0.5, -0.5, 0 }, .col { 1, 1, 1, 1 }, .tex { _tx, _ty } },
            Vertex{ .pos {  0.5,  0.5, 0 }, .col { 1, 1, 1, 1 }, .tex { _tw, _th } },
            Vertex{ .pos { -0.5,  0.5, 0 }, .col { 1, 1, 1, 1 }, .tex { _tx, _th } },
            
            Vertex{ .pos { -0.5, -0.5, 0 }, .col { 1, 1, 1, 1 }, .tex { _tx, _ty } },
            Vertex{ .pos {  0.5, -0.5, 0 }, .col { 1, 1, 1, 1 }, .tex { _tw, _ty } },
            Vertex{ .pos {  0.5,  0.5, 0 }, .col { 1, 1, 1, 1 }, .tex { _tw, _th } },
        };
    }


    template<std::size_t N>
    mem::BlockPtr<box::Array<Vertex, (N-1) * 6>> CreateVerticesFromText(const char (&text)[N])
    {
        auto blockPtr = mem::ClaimBlock<box::Array<Vertex, (N-1) * 6>>();

        FOR_CARRAY(text, i){
            if (text[i] == '\0') continue;

            const auto& coords = MAPPING.GetValue(text[i]);
            const auto  quad   = CreateQuad(0, 0, 14, 18, coords[math::Vx], coords[math::Vy]);
            //LETTER_SIZE[math::Vx], LETTER_SIZE[math::Vy]
            blockPtr->AppendArray(quad.vertices);

        }
        
        return blockPtr;
    }    

}//ns