//https://github.com/awwdev

#pragma once
#include "gpu/RenderData/RenderData.hpp"

#include "com/DeltaTime.hpp"
#include "com/Algorithms.hpp"
#include "com/box/EnumMap.hpp"

#include "wnd/WindowEvents.hpp"
#include "com/box/String.hpp"
#include "dbg/Logger.hpp"
#include "app/InputMode.hpp"

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
    ENUM_END
};

const com::EnumMap<Colors::ENUM_END, com::Vec4f> ColorValues = {
    { Colors::White,        { 0.90, 0.90, 0.90, 1.0 } },
    { Colors::Green,        { 0.01, 0.90, 0.01, 1.0 } },
    { Colors::Red,          { 0.90, 0.01, 0.01, 1.0 } },
    { Colors::Black1,       { 0.01, 0.01, 0.01, 1.0 } },
    { Colors::Black2_Alpha, { 0.00, 0.00, 0.00, 0.7 } },
    { Colors::Black3,       { 0.03, 0.03, 0.03, 1.0 } },
    { Colors::Black4,       { 0.04, 0.04, 0.04, 1.0 } },
    { Colors::Black5,       { 0.05, 0.05, 0.05, 1.0 } },
    { Colors::Orange,       { 235/255.f, 140/255.f, 52/255.f, 1.0 } },
};

//? RECTANGLE

inline void AddRect(gpu::RenderData& renderData, 
const com::Rectf& rect, const Colors col, 
const u32 texIdx = FULL_OPAQUE_NO_TEXTURE, const bool blur = false)
{
    const com::Rectf normRect = { 
        //vulkan space
        (rect.x / (f32)wnd::glo::window_w) * 2 - 1,  
        (rect.y / (f32)wnd::glo::window_h) * 2 - 1,  
        (rect.width / (f32)wnd::glo::window_w) * 2,  
        (rect.height / (f32)wnd::glo::window_h) * 2,  
    };

    renderData.gui.uboText.Append(gpu::RenderData_GUI::UBO_Text{ 
        .rect    = normRect,
        .visuals = {
            .colIdx = (u32)col,
            .texIdx = (u32)texIdx,
        }
    });

    if (blur) 
    {
        using namespace com;
        com::Vec2f v1 = { rect.x, rect.y };
        com::Vec2f v2 = { rect.x + rect.width, rect.y };
        com::Vec2f v3 = { rect.x + rect.width, rect.y + rect.height };
        com::Vec2f v4 = { rect.x, rect.y + rect.height };
        const auto uv1 = com::Vec2f{ v1.x / wnd::glo::window_w, v1.y / wnd::glo::window_h };
        const auto uv2 = com::Vec2f{ v2.x / wnd::glo::window_w, v2.y / wnd::glo::window_h };
        const auto uv3 = com::Vec2f{ v3.x / wnd::glo::window_w, v3.y / wnd::glo::window_h };
        const auto uv4 = com::Vec2f{ v4.x / wnd::glo::window_w, v4.y / wnd::glo::window_h };
        v1 = uv1 * 2 - 1;
        v2 = uv2 * 2 - 1;
        v3 = uv3 * 2 - 1;
        v4 = uv4 * 2 - 1;

        gpu::RenderData_Post::Vertex verts [] {
            {v1, uv1, 1},
            {v2, uv2, 1},
            {v3, uv3, 1},
            {v1, uv1, 1},
            {v3, uv3, 1},
            {v4, uv4, 1},
        };
        renderData.post.vboBlur.AppendArray(verts);
    }
}

inline void AddRectOutline(gpu::RenderData& renderData, 
const com::Rectf& rect, const Colors col, const f32 thickness = 1)
{
    //lines
    const com::Rectf t { rect.x, rect.y, rect.width, thickness };
    const com::Rectf r { rect.x + rect.width - thickness, rect.y, thickness, rect.height };
    const com::Rectf b { rect.x, rect.y + rect.height - thickness, rect.width, thickness };
    const com::Rectf l { rect.x, rect.y, thickness, rect.height };

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
        const com::Rectf rect = { x + LETTER_SPACE * i, y, LETTER_SIZE, LETTER_SIZE };
        AddRect(renderData, rect, col, (u32)str[i]);
    }
}

inline void AddTextCentered(gpu::RenderData& renderData, 
chars_t str, const idx_t len, const com::Rectf& center, const Colors col = Colors::White)
{
    const auto strWidth = len * LETTER_SPACE; //+1 ?
    const auto x = center.x + center.width * 0.5f - strWidth    * 0.5f;
    const auto y = center.y + center.height * 0.5f - LETTER_SIZE * 0.5f;
    AddText(renderData, str, len, x, y, col);
}

template<idx_t N>
inline void AddText(gpu::RenderData& renderData, 
const com::String<N>& str, const f32 x, const f32 y, const Colors col = Colors::White)
{
    AddText(renderData, str.cstr, str.length, x, y, col);
}

template<idx_t N>
inline void AddTextCentered(gpu::RenderData& renderData, 
const com::String<N>& str, const com::Rectf& center, const Colors col = Colors::White)
{
    AddTextCentered(renderData, str.cstr, str.length, center, col);
}

}//ns