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
        //!y is from bottom to top
        P { 'H', { 4, 4 } },
        P { 'E', { 1, 4 } },
        P { 'Y', { 3, 3 } },
    };  
    const math::Vec2f LETTER_SIZE { 14, 18 };


    struct Quad { Vertex vertices [6]; };

    inline Quad CreateQuad(
        const f32 x,  const f32 y, 
        const f32 w,  const f32 h,
        const f32 tx, const f32 ty)
    {
        //letter size
        const f32 lw = 14;
        const f32 lh = 18;

        const f32 _tx = tx * (lw/w);
        const f32 _ty = ty * (lh/h);
        const f32 _tw = _tx + (lw/w);
        const f32 _th = _ty - (lh/h);

        const f32 s = 0.25f;
        const f32 xo = x * s;
        const f32 yo = 0.f;

        return {
            Vertex{ .pos { -1 * s + xo, -1 * s + yo, 0 }, .col { 1, 1, 1, 1 }, .tex { _tx, _ty } },
            Vertex{ .pos {  1 * s + xo,  1 * s + yo, 0 }, .col { 1, 1, 1, 1 }, .tex { _tw, _th } },
            Vertex{ .pos { -1 * s + xo,  1 * s + yo, 0 }, .col { 1, 1, 1, 1 }, .tex { _tx, _th } },
            
            Vertex{ .pos { -1 * s + xo, -1 * s + yo, 0 }, .col { 1, 1, 1, 1 }, .tex { _tx, _ty } },
            Vertex{ .pos {  1 * s + xo, -1 * s + yo, 0 }, .col { 1, 1, 1, 1 }, .tex { _tw, _ty } },
            Vertex{ .pos {  1 * s + xo,  1 * s + yo, 0 }, .col { 1, 1, 1, 1 }, .tex { _tw, _th } },
        };
    }


    template<std::size_t N>
    mem::BlockPtr<box::Array<Vertex, (N-1) * 6>> CreateVerticesFromText(const char (&text)[N])
    {
        auto blockPtr = mem::ClaimBlock<box::Array<Vertex, (N-1) * 6>>();

        FOR_CARRAY(text, i){
            if (text[i] == '\0') continue;

            const auto& coords = MAPPING.GetValue(text[i]);
            const auto  quad   = CreateQuad(i, 0, 252, 108, coords[math::Vx], coords[math::Vy]);
            //LETTER_SIZE[math::Vx], LETTER_SIZE[math::Vy]
            blockPtr->AppendArray(quad.vertices);

        }
        
        return blockPtr;
    }    

}//ns