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
        P { ' ', {  0,  6 } },
        P { '!', {  1,  6 } },
        P { '"', {  2,  6 } },
        P { '#', {  3,  6 } },
        P { '$', {  4,  6 } },
        P { '%', {  5,  6 } },
        P { '&', {  6,  6 } },
        P { '\'', {  7,  6 } },
        P { '(', {  8,  6 } },
        P { ')', {  9,  6 } },
        P { '*', { 10,  6 } },
        P { '+', { 11,  6 } },
        P { ',', { 12,  6 } },
        P { '-', { 13,  6 } },
        P { '.', { 14,  6 } },
        P { '/', { 15,  6 } },
        P { '0', { 16,  6 } },
        P { '1', { 17,  6 } },

        P { '2', {  0,  5 } },
        P { '3', {  1,  5 } },
        P { '4', {  2,  5 } },
        P { '5', {  3,  5 } },
        P { '6', {  4,  5 } },
        P { '7', {  5,  5 } },
        P { '8', {  6,  5 } },
        P { '9', {  7,  5 } },
        P { ':', {  8,  5 } },
        P { ';', {  9,  5 } },
        P { '<', { 10,  5 } },
        P { '=', { 11,  5 } },
        P { '>', { 12,  5 } },
        P { '?', { 13,  5 } },
        P { '@', { 14,  5 } },
        P { 'A', { 15,  5 } },
        P { 'B', { 16,  5 } },
        P { 'C', { 17,  5 } },

        P { 'D', {  0,  4 } },
        P { 'E', {  1,  4 } },
        P { 'F', {  2,  4 } },
        P { 'G', {  3,  4 } },
        P { 'H', {  4,  4 } },
        P { 'I', {  5,  4 } },
        P { 'J', {  6,  4 } },
        P { 'K', {  7,  4 } },
        P { 'L', {  8,  4 } },
        P { 'M', {  9,  4 } },
        P { 'N', { 10,  4 } },
        P { 'O', { 11,  4 } },
        P { 'P', { 12,  4 } },
        P { 'Q', { 13,  4 } },
        P { 'R', { 14,  4 } },
        P { 'S', { 15,  4 } },
        P { 'T', { 16,  4 } },
        P { 'U', { 17,  4 } },

        P { 'V', {  0,  3 } },
        P { 'W', {  1,  3 } },
        P { 'X', {  2,  3 } },
        P { 'Y', {  3,  3 } },
        P { 'Z', {  4,  3 } },
        P { '[', {  5,  3 } },
        P { '\\', {  6,  3 } },
        P { ']', {  7,  3 } },
        P { '^', {  8,  3 } },
        P { '_', {  9,  3 } },
        P { '`', { 10,  3 } },
        P { 'a', { 11,  3 } },
        P { 'b', { 12,  3 } },
        P { 'c', { 13,  3 } },
        P { 'd', { 14,  3 } },
        P { 'e', { 15,  3 } },
        P { 'f', { 16,  3 } },
        P { 'g', { 17,  3 } },

        P { 'h', {  0, 2 } },
        P { 'i', {  1,  2 } },
        P { 'j', {  2,  2 } },
        P { 'k', {  3,  2 } },
        P { 'l', {  4,  2 } },
        P { 'm', {  5,  2 } },
        P { 'n', {  6,  2 } },
        P { 'o', {  7,  2 } },
        P { 'p', {  8,  2 } },
        P { 'q', {  9,  2 } },
        P { 'r', { 10,  2 } },
        P { 's', { 11,  2 } },
        P { 't', { 12,  2 } },
        P { 'u', { 13,  2 } },
        P { 'v', { 14,  2 } },
        P { 'w', { 15,  2 } },
        P { 'x', { 16,  2 } },
        P { 'y', { 17,  2 } },

        P { 'z', {  1,  1 } },
        P { '{', {  2,  1 } },
        P { '|', {  3,  1 } },
        P { '}', {  4,  1 } },
        P { '~', {  5,  1 } },

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
        const f32 xo = 2*((pos_x*fs)/wnd_h) - 1;
        const f32 yo = 2*(1/wnd_w) - 1;
        
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
    mem::BlockPtr<box::Array<Vertex, (N-1) * 6>> CreateVerticesFromText(const char (&text)[N])
    {
        auto blockPtr = mem::ClaimBlock<box::Array<Vertex, (N-1) * 6>>();

        FOR_CARRAY(text, i){
            if (text[i] == '\0') continue; //!this is why N-1

            const auto& coords = MAPPING.GetValue(text[i]);
            const auto  quad   = CreateQuad(
                i, 0, 
                252, 108, 
                coords[math::Vx], coords[math::Vy],
                14, 18,
                600, 800,
                { 1, 1, 1, 1 }
            );
            //TODO: get window size
            //TODO: index buffer
            //TODO: make independent of aspect ratio - no distortion
            blockPtr->AppendArray(quad.vertices);

        }
        
        return blockPtr;
    }    

}//ns