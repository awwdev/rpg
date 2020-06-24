#pragma once

#include "mini/Utils/Structs.hpp"
#include "mini/Memory/Allocator.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Utils/Types.hpp"
#include "mini/Utils/Algorithms.hpp"

namespace mini::mesh
{
    struct VerticesQuad4 { utils::Vertex verts [4]; };
    struct VerticesQuad6 { utils::Vertex verts [6]; };

    enum class Indexed  { Yes, No };
    enum class Centered { Yes, No }; //TODO: impl

    template<Indexed IsIndexed = Indexed::No>
    inline auto CreateVertices_Rect(
        const utils::Rect<f32>& pos,
        const utils::Rect<f32>& tex,
        const utils::NormColor4f& col = { 1, 1, 1, 1 })
    {
        using namespace utils;

        const Vertex tl { .pos { pos.x          , pos.y         , 0 }, .col { col }, .tex { tex.x        , tex.y + tex.h } };
        const Vertex tr { .pos { pos.x + pos.w  , pos.y         , 0 }, .col { col }, .tex { tex.x + tex.w, tex.y + tex.h } };
        const Vertex br { .pos { pos.x + pos.w  , pos.y + pos.h , 0 }, .col { col }, .tex { tex.x + tex.w, tex.y         } };
        const Vertex bl { .pos { pos.x          , pos.y + pos.h , 0 }, .col { col }, .tex { tex.x        , tex.y         } };

        if constexpr (IsIndexed == Indexed::Yes) return VerticesQuad4 { tl, tr, br, bl };           //0123
        if constexpr (IsIndexed == Indexed::No)  return VerticesQuad6 { tl, br, bl, tl, tr, br };   //023012
    }
    
}//ns