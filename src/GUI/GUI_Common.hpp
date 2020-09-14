//https://github.com/awwdev

#pragma once
#include "GPU/RenderData.hpp"

#include "Common/Structs.hpp"
#include "Common/DeltaTime.hpp"
#include "Common/Algorithms.hpp"

#include "Window/WindowEvents.hpp"
#include "Debug/Logger.hpp"
#include "Common/Container/String.hpp"
#include "App/InputMode.hpp"

namespace rpg::gui {

constexpr idx_t LETTER_SIZE  = 16; //monospace
constexpr idx_t LETTER_SPACE = 8;
constexpr idx_t FULL_OPAQUE_NO_TEXTURE = 21; //using NAK ascii code 
constexpr f32   LINE_HEIGHT = 18;
constexpr f32   PADDING = 4;

enum class Colors : u32
{
    White,
    Green,
    Red,
    Black1,
    Black2_Alpha,
    Black3,
    Black4,
    Black5,
    Orange,
};

//? RECTANGLE

inline void AddRect(gpu::RenderData& renderData, 
const com::Rect<f32>& rect, const Colors col, const u32 texIdx = FULL_OPAQUE_NO_TEXTURE, bool blur = false)
{
    renderData.gui_ubo.AppendData({ 
        .rect         = rect,
        .colorIndex   = (u32)col,
        .textureIndex = texIdx,
    });
    if (blur) {
        using namespace com;
        com::Vec2f v1 = { rect.x, rect.y };
        com::Vec2f v2 = { rect.x + rect.w, rect.y };
        com::Vec2f v3 = { rect.x + rect.w, rect.y + rect.h };
        com::Vec2f v4 = { rect.x, rect.y + rect.h };
        const auto uv1 = com::Vec2f{ v1[X] / wnd::glo::window_w, v1[Y] / wnd::glo::window_h };
        const auto uv2 = com::Vec2f{ v2[X] / wnd::glo::window_w, v2[Y] / wnd::glo::window_h };
        const auto uv3 = com::Vec2f{ v3[X] / wnd::glo::window_w, v3[Y] / wnd::glo::window_h };
        const auto uv4 = com::Vec2f{ v4[X] / wnd::glo::window_w, v4[Y] / wnd::glo::window_h };
        v1 = uv1 * 2 - 1;
        v2 = uv2 * 2 - 1;
        v3 = uv3 * 2 - 1;
        v4 = uv4 * 2 - 1;

        com::Post_Vertex verts [] {
            {v1, uv1, 1},
            {v2, uv2, 1},
            {v3, uv3, 1},
            {v1, uv1, 1},
            {v3, uv3, 1},
            {v4, uv4, 1},
        };
        renderData.gui_vbo_blur.AppendArray(verts);
    }
}

inline void AddRectOutline(gpu::RenderData& renderData, 
const com::Rect<f32>& rect, const Colors col, const f32 thickness = 1)
{
    //lines
    const com::Rect<f32> t { rect.x, rect.y, rect.w, thickness };
    const com::Rect<f32> r { rect.x + rect.w, rect.y, thickness, rect.h };
    const com::Rect<f32> b { rect.x, rect.y + rect.h, rect.w, thickness };
    const com::Rect<f32> l { rect.x, rect.y, thickness, rect.h };

    AddRect(renderData, t, col);
    AddRect(renderData, r, col);
    AddRect(renderData, b, col);
    AddRect(renderData, l, col);
}

//? TEXT 

inline void AddText(gpu::RenderData& renderData, 
chars_t str, const idx_t len, const f32 x, const f32 y, const Colors col = Colors::White)
{
    for(idx_t i = 0; i < len; ++i) {
        const com::Rect<f32> rect = { x + LETTER_SPACE * i, y, LETTER_SIZE, LETTER_SIZE };
        AddRect(renderData, rect, col, (u32)str[i]);
    }
}

inline void AddTextCentered(gpu::RenderData& renderData, 
chars_t str, const idx_t len, const com::Rect<f32>& center, const Colors col = Colors::White)
{
    const auto strWidth = len * LETTER_SPACE; //+1 ?
    const auto x = center.x + center.w * 0.5f - strWidth    * 0.5f;
    const auto y = center.y + center.h * 0.5f - LETTER_SIZE * 0.5f;
    AddText(renderData, str, len, x, y, col);
}

template<idx_t N>
inline void AddText(gpu::RenderData& renderData, 
const com::String<N>& str, const f32 x, const f32 y, const Colors col = Colors::White)
{
    AddText(renderData, str.data, str.length, x, y, col);
}

template<idx_t N>
inline void AddTextCentered(gpu::RenderData& renderData, 
const com::String<N>& str, const com::Rect<f32>& center, const Colors col = Colors::White)
{
    AddTextCentered(renderData, str.data, str.length, center, col);
}

}//ns