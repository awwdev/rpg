#pragma once

#include "mini/Box/Array.hpp"
#include "mini/Box/String.hpp"
#include "mini/Box/Map.hpp"
#include "mini/Utils/Vertex.hpp"
#include "mini/Memory/Allocator.hpp"
#include "mini/Math/Matrix.hpp"
#include "mini/Resources/Mesh/PrimitiveMeshes.hpp"

namespace mini::res
{
    using M = box::Map<math::Vec<int, 2>, 256>;
    using P = M::Pair_t;
    const M MAPPING
    {
        //!y is from bottom to top
        P { ' ', {  0,  5 } },
        P { '!', {  1,  5 } },
        P { '"', {  2,  5 } },
        P { '#', {  3,  5 } },
        P { '$', {  4,  5 } },
        P { '%', {  5,  5 } },
        P { '&', {  6,  5 } },
        P { '\'', {  7,  5 } },
        P { '(', {  8,  5 } },
        P { ')', {  9,  5 } },
        P { '*', { 10,  5 } },
        P { '+', { 11,  5 } },
        P { ',', { 12,  5 } },
        P { '-', { 13,  5 } },
        P { '.', { 14,  5 } },
        P { '/', { 15,  5 } },
        P { '0', { 16,  5 } },
        P { '1', { 17,  5 } },

        P { '2', {  0,  4 } },
        P { '3', {  1,  4 } },
        P { '4', {  2,  4 } },
        P { '5', {  3,  4 } },
        P { '6', {  4,  4 } },
        P { '7', {  5,  4 } },
        P { '8', {  6,  4 } },
        P { '9', {  7,  4 } },
        P { ':', {  8,  4 } },
        P { ';', {  9,  4 } },
        P { '<', { 10,  4 } },
        P { '=', { 11,  4 } },
        P { '>', { 12,  4 } },
        P { '?', { 13,  4 } },
        P { '@', { 14,  4 } },
        P { 'A', { 15,  4 } },
        P { 'B', { 16,  4 } },
        P { 'C', { 17,  4 } },

        P { 'D', {  0,  3 } },
        P { 'E', {  1,  3 } },
        P { 'F', {  2,  3 } },
        P { 'G', {  3,  3 } },
        P { 'H', {  4,  3 } },
        P { 'I', {  5,  3 } },
        P { 'J', {  6,  3 } },
        P { 'K', {  7,  3 } },
        P { 'L', {  8,  3 } },
        P { 'M', {  9,  3 } },
        P { 'N', { 10,  3 } },
        P { 'O', { 11,  3 } },
        P { 'P', { 12,  3 } },
        P { 'Q', { 13,  3 } },
        P { 'R', { 14,  3 } },
        P { 'S', { 15,  3 } },
        P { 'T', { 16,  3 } },
        P { 'U', { 17,  3 } },

        P { 'V', {  0,  2 } },
        P { 'W', {  1,  2 } },
        P { 'X', {  2,  2 } },
        P { 'Y', {  3,  2 } },
        P { 'Z', {  4,  2 } },
        P { '[', {  5,  2 } },
        P { '\\', {  6, 2  } },
        P { ']', {  7,  2 } },
        P { '^', {  8,  2 } },
        P { '_', {  9,  2 } },
        P { '`', { 10,  2 } },
        P { 'a', { 11,  2 } },
        P { 'b', { 12,  2 } },
        P { 'c', { 13,  2 } },
        P { 'd', { 14,  2 } },
        P { 'e', { 15,  2 } },
        P { 'f', { 16,  2 } },
        P { 'g', { 17,  2 } },

        P { 'h', {  0,  1 } },
        P { 'i', {  1,  1 } },
        P { 'j', {  2,  1 } },
        P { 'k', {  3,  1 } },
        P { 'l', {  4,  1 } },
        P { 'm', {  5,  1 } },
        P { 'n', {  6,  1 } },
        P { 'o', {  7,  1 } },
        P { 'p', {  8,  1 } },
        P { 'q', {  9,  1 } },
        P { 'r', { 10,  1 } },
        P { 's', { 11,  1 } },
        P { 't', { 12,  1 } },
        P { 'u', { 13,  1 } },
        P { 'v', { 14,  1 } },
        P { 'w', { 15,  1 } },
        P { 'x', { 16,  1 } },
        P { 'y', { 17,  1 } },

        P { 'z', {  1,  0 } },
        P { '{', {  2,  0 } },
        P { '|', {  3,  0 } },
        P { '}', {  4,  0 } },
        P { '~', {  5,  0 } },
    };  

    struct Quad { Vertex vertices [6]; };

    inline Quad CreateQuad(
        const f32 pos_x, const f32 pos_y, 
        const f32 tex_w, const f32 tex_h,
        const f32 cel_x, const f32 cel_y,
        const f32 let_w, const f32 let_h, 
        const f32 wnd_w, const f32 wnd_h,
        const math::Vec4f col)
    {
        const f32 _tx = cel_x * (let_w/tex_w);
        const f32 _ty = cel_y * (let_h/tex_h);
        const f32 _tw = _tx   + (let_w/tex_w);
        const f32 _th = _ty   - (let_h/tex_h);

        const f32 fs = 20; 
        const f32 s = fs/wnd_w;
        const f32 xo = 2*((pos_x*fs)/wnd_w) - 1;
        const f32 yo = 2*(1/wnd_h) - 1;
        
        return {
            Vertex{ .pos { 0 * s + xo, 0 * s + yo, 0 }, .col { col }, .tex { _tx, _ty } },
            Vertex{ .pos { 1 * s + xo, 1 * s + yo, 0 }, .col { col }, .tex { _tw, _th } },
            Vertex{ .pos { 0 * s + xo, 1 * s + yo, 0 }, .col { col }, .tex { _tx, _th } },
            
            Vertex{ .pos { 0 * s + xo, 0 * s + yo, 0 }, .col { col }, .tex { _tx, _ty } },
            Vertex{ .pos { 1 * s + xo, 0 * s + yo, 0 }, .col { col }, .tex { _tw, _ty } },
            Vertex{ .pos { 1 * s + xo, 1 * s + yo, 0 }, .col { col }, .tex { _tw, _th } },
        };
    }


    template<std::size_t N>
    auto CreateVerticesFromText(const char (&text)[N])
    //-> mem::BlockPtr<Vertex[(N-1) * 6]>
    {
        auto blockPtr = mem::ClaimBlock<box::Array<Vertex, (N-1) * 6>>();

        FOR_CARRAY(text, i){
            if (text[i] == '\0') continue; //!this is why N-1

            const auto fw = 14*2;
            const auto fh = 18*2; 

            const auto& coords = MAPPING.GetValue(text[i]);
            const auto quad = res::CreateRect(
                Rect<int>{i * fw, 0, fw, fh}, 
                Rect<int>{coords[math::Vx] * 14, coords[math::Vy] * 18, 14, 18}
            );

            blockPtr->AppendArray(quad.verts);

        }
        
        return blockPtr;
    }    

    //? basic idea: do all in px on cpu side and via push constant transfer width and height of wnd an divide all verts
    //TODO: get window size
    //TODO: index buffer
    //TODO: make independent of aspect ratio - no distortion

}//ns