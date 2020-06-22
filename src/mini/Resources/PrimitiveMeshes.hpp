#pragma once

#include "mini/Utils/Structs.hpp"
#include "mini/Memory/Allocator.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Utils/Types.hpp"
#include "mini/Utils/Algorithms.hpp"

namespace mini::res
{
    struct Quad4 { utils::Vertex verts [4]; };
    struct Quad6 { utils::Vertex verts [6]; };

    enum class Indexed  { Yes, No };
    enum class Centered { Yes, No };

    //in pixel 
    template<Indexed IsIndexed = Indexed::No>
    inline auto CreateRect(
        const utils::Rect<int> pos, //TODO: actually we want pixel so int, but
        const utils::Rect<int> tex,
        const math::Vec4f col = { 1, 1, 1, 1 }
    )
    {
        const utils::Vertex tl { .pos { (f32)pos.x          , (f32)pos.y         , 0 }, .col { col }, .tex { (f32)tex.x           , (f32)tex.y + tex.h } };
        const utils::Vertex tr { .pos { (f32)pos.x + pos.w  , (f32)pos.y         , 0 }, .col { col }, .tex { (f32)tex.x + tex.w   , (f32)tex.y + tex.h} };
        const utils::Vertex br { .pos { (f32)pos.x + pos.w  , (f32)pos.y + pos.h , 0 }, .col { col }, .tex { (f32)tex.x + tex.w   , (f32)tex.y } };
        const utils::Vertex bl { .pos { (f32)pos.x          , (f32)pos.y + pos.h , 0 }, .col { col }, .tex { (f32)tex.x           , (f32)tex.y } };

        if constexpr (IsIndexed == Indexed::Yes) return Quad4 { tl, tr, br, bl };           //0123
        if constexpr (IsIndexed == Indexed::No)  return Quad6 { tl, br, bl, tl, tr, br };   //023012
    }
    
}//ns