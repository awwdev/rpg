#pragma once

#include "mini/Utils/Vertex.hpp"
#include "mini/Memory/Allocator.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Utils/Types.hpp"

namespace mini::res
{
    struct Quad4 { Vertex verts [4]; };
    struct Quad6 { Vertex verts [6]; };

    enum class Indexed  { Yes, No };
    enum class Centered { Yes, No };

    //do all in px on cpu side and via push constant we deliver wnd size and devide vert pos in shader
    
    //in pixel 
    template<Indexed IsIndexed = Indexed::No>
    inline auto CreateRect(
        const Rect<int> pos, //TODO: actually we want pixel so int, but
        const Rect<int> tex,
        const math::Vec4f col = { 1, 1, 1, 1 }
    )
    {
        const Vertex tl { .pos { (f32)pos.x          , (f32)pos.y         , 0 }, .col { col }, .tex { (f32)tex.x           , (f32)tex.y + tex.h } };
        const Vertex tr { .pos { (f32)pos.x + pos.w  , (f32)pos.y         , 0 }, .col { col }, .tex { (f32)tex.x + tex.w   , (f32)tex.y + tex.h} };
        const Vertex br { .pos { (f32)pos.x + pos.w  , (f32)pos.y + pos.h , 0 }, .col { col }, .tex { (f32)tex.x + tex.w   , (f32)tex.y } };
        const Vertex bl { .pos { (f32)pos.x          , (f32)pos.y + pos.h , 0 }, .col { col }, .tex { (f32)tex.x           , (f32)tex.y } };

        if constexpr (IsIndexed == Indexed::Yes) return Quad4 { tl, tr, br, bl };
        if constexpr (IsIndexed == Indexed::No)  return Quad6 { tl, br, bl, tl, tr, br };
    }
    
}//ns