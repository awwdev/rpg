#pragma once

#include "mini/Utils/Structs.hpp"
#include "mini/Memory/Allocator.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Utils/Types.hpp"
#include "mini/Utils/Algorithms.hpp"

namespace mini::mesh
{
    struct Quad4 { utils::Vertex verts [4]; };
    struct Quad6 { utils::Vertex verts [6]; };

    enum class Indexed  { Yes, No };
    enum class Centered { Yes, No };

    //in pixel 
    //!how would world pos look like?
    template<Indexed IsIndexed = Indexed::No>
    inline auto CreateRect(
        const utils::Rect<f32>& pos,
        const utils::Rect<f32>& tex,
        const utils::NormColor4f& col = { 1, 1, 1, 1 })
    {
        using namespace utils;

        const Vertex tl { .pos { pos.x          , pos.y         , 0 }, .col { col }, .tex { tex.x        , tex.y + tex.h } };
        const Vertex tr { .pos { pos.x + pos.w  , pos.y         , 0 }, .col { col }, .tex { tex.x + tex.w, tex.y + tex.h } };
        const Vertex br { .pos { pos.x + pos.w  , pos.y + pos.h , 0 }, .col { col }, .tex { tex.x + tex.w, tex.y         } };
        const Vertex bl { .pos { pos.x          , pos.y + pos.h , 0 }, .col { col }, .tex { tex.x        , tex.y         } };

        if constexpr (IsIndexed == Indexed::Yes) return Quad4 { tl, tr, br, bl };           //0123
        if constexpr (IsIndexed == Indexed::No)  return Quad6 { tl, br, bl, tl, tr, br };   //023012
    }
    
}//ns