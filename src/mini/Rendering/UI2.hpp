//https://github.com/awwdev

#pragma once
#include "mini/Rendering/RenderGraph.hpp"
#include "mini/Utils/Structs.hpp"

namespace mini::ui2 {

//! GLOBAL CONSISTENCY

//analog to the shader color lookup table
enum Colors : u32
{
    WHITE,
    GREEN,
    RED,
    BLACK1,
    BLACK2,
    BLACK3,
    BLACK4,
    BLACK5,
};

constexpr idx_t LETTER_SIZE  = 16;
constexpr idx_t LETTER_SPACE = 16;
constexpr idx_t ASCII_OFFSET = 32;
constexpr idx_t FULL_OPAQUE  = 95;

//! GPU UBO

inline void AddString(
    rendering::RenderGraph& renderGraph, 
    const f32 x, const f32 y, 
    chars_t str, const idx_t len,
    const Colors col = Colors::WHITE)
{
    for(idx_t i = 0; i < len; ++i) {
        renderGraph.ui_ubo.AppendData({ 
            .rect         = { x + LETTER_SPACE * i, y, LETTER_SIZE, LETTER_SIZE }, //monospace
            .colorIndex   = col,
            .textureIndex = str[i] != '\0' ? str[i] - ASCII_OFFSET : ' ' - ASCII_OFFSET
        });
    }
}

//! SPECIFIC

inline void DrawFPS(RenderGraph& renderGraph, const utils::Rect<float>& rect = { 0, 0, 48, 20 })
{
    renderGraph.ui_ubo.AppendData(
        rendering::UI_UniformData { 
            .rect           = rect,
            .colorIndex     = BLACK1, 
            .textureIndex   = FULL_OPAQUE
        }
    );

    char fpsStr [10] { '\0 '};
    const auto res = std::to_chars(fpsStr, fpsStr + 10, dt::fps);
    DrawTextCentered(renderGraph, rect, fpsStr, (u32)strlen(fpsStr), GREEN);
}



} //ns