//https://github.com/awwdev

#pragma once
#include "Rendering/RenderData.hpp"

#include "Utils/Structs.hpp"
#include "Utils/DeltaTime.hpp"
#include "Utils/Algorithms.hpp"

#include "Window/WindowEvents.hpp"
#include "Debug/Logger.hpp"
#include "Box/String.hpp"
#include "App/InputMode.hpp"

namespace rpg::dbg::gui {

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
};

//? RECTANGLE

inline void AddRect(gpu::RenderData& renderData, 
const use::Rect<f32>& rect, const Colors col, const u32 texIdx = FULL_OPAQUE_NO_TEXTURE)
{
    renderData.gui_ubo.AppendData({ 
        .rect         = rect,
        .colorIndex   = (u32)col,
        .textureIndex = texIdx,
    });
}

inline void AddRectOutline(gpu::RenderData& renderData, 
const use::Rect<f32>& rect, const Colors col, const f32 thickness = 1)
{
    //lines
    const use::Rect<f32> t { rect.x, rect.y, rect.w, thickness };
    const use::Rect<f32> r { rect.x + rect.w, rect.y, thickness, rect.h };
    const use::Rect<f32> b { rect.x, rect.y + rect.h, rect.w, thickness };
    const use::Rect<f32> l { rect.x, rect.y, thickness, rect.h };

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
        const use::Rect<f32> rect = { x + LETTER_SPACE * i, y, LETTER_SIZE, LETTER_SIZE };
        AddRect(renderData, rect, col, (u32)str[i]);
    }
}

inline void AddTextCentered(gpu::RenderData& renderData, 
chars_t str, const idx_t len, const use::Rect<f32>& center, const Colors col = Colors::White)
{
    const auto strWidth = len * LETTER_SPACE; //+1 ?
    const auto x = center.x + center.w * 0.5f - strWidth    * 0.5f;
    const auto y = center.y + center.h * 0.5f - LETTER_SIZE * 0.5f;
    AddText(renderData, str, len, x, y, col);
}

template<idx_t N>
inline void AddText(gpu::RenderData& renderData, 
const box::String<N>& str, const f32 x, const f32 y, const Colors col = Colors::White)
{
    AddText(renderData, str.data, str.length, x, y, col);
}

template<idx_t N>
inline void AddTextCentered(gpu::RenderData& renderData, 
const box::String<N>& str, const use::Rect<f32>& center, const Colors col = Colors::White)
{
    AddTextCentered(renderData, str.data, str.length, center, col);
}

}//ns