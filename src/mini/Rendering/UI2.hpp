//https://github.com/awwdev

#pragma once
#include "mini/Rendering/RenderGraph.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/Utils/DeltaTime.hpp"
#include "mini/Utils/Algorithms.hpp"
#include "mini/Window/WindowEvents.hpp"
#include "mini/Box/String.hpp"

#include <charconv>

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

constexpr idx_t LETTER_SIZE  = 16; //monospace
constexpr idx_t LETTER_SPACE = 8;
constexpr idx_t FULL_OPAQUE_NO_TEXTURE = 21; //using NAK ascii code 
constexpr f32   LINE_HEIGHT = 16;

//! BASIC

inline void DrawText(
    rendering::RenderGraph& renderGraph, 
    const f32 x, const f32 y, 
    chars_t str, const idx_t len,
    const Colors col = Colors::WHITE)
{
    for(idx_t i = 0; i < len; ++i) {
        renderGraph.ui_ubo.AppendData({ 
            .rect         = { x + LETTER_SPACE * i, y, LETTER_SIZE, LETTER_SIZE },
            .colorIndex   = col,
            .textureIndex = (u32)str[i]
        });
    }
}

inline void DrawRectangle(rendering::RenderGraph& renderGraph, utils::Rect<f32> rect, const Colors col)
{
    renderGraph.ui_ubo.AppendData({ 
        .rect         = rect,
        .colorIndex   = col,
        .textureIndex = FULL_OPAQUE_NO_TEXTURE
    });
}

//! ADVANCED

inline void DrawTextCentered(
    rendering::RenderGraph& renderGraph,
    chars_t str, const idx_t len, 
    const utils::Rect<f32>& rect, 
    const Colors col = WHITE)
{
    const auto TOTAL_STR_W = (len + 1) * LETTER_SPACE;
    const auto cx = rect.x + rect.w * 0.5f - TOTAL_STR_W * 0.5f;
    const auto cy = rect.y + rect.h * 0.5f - LETTER_SIZE * 0.5f;

    DrawText(renderGraph, cx, cy, str, len, col);
}

//! WINDOW

struct Window
{
    static constexpr auto TOP_BAR_H = 20;

    box::String<20>  title;
    utils::Rect<f32> rect {};
    utils::Rect<f32> limits = { rect.w, rect.h, f32max, f32max };

    enum Mode { None, Resize, Move } mode = None;
    u32  dragStartX {}, dragStartY {}; 
    bool isDragging {};
};

inline void DrawWindow(rendering::RenderGraph& renderGraph, Window& wnd)
{
    using namespace utils;
    using namespace wnd;

    const utils::Rect<f32> bar     = { wnd.rect.x, wnd.rect.y, wnd.rect.w, wnd.TOP_BAR_H };
    const utils::Rect<f32> resizer = { wnd.rect.x + wnd.rect.w - 8, wnd.rect.y + wnd.rect.h - 8, 8, 8 };

    const bool isMouseOnBar     = IsPointInsideRect(global::mouse_wx, global::mouse_wy, bar);
    const bool isMouseOnResizer = IsPointInsideRect(global::mouse_wx, global::mouse_wy, resizer);

    //INTERACTION

    if (HasEvent<Mouse_ButtonLeft, Released>())
        wnd.mode = Window::Mode::None;

    if (HasEvent<Mouse_ButtonLeft, Pressed>()) {        
        wnd.dragStartX = global::mouse_wx;
        wnd.dragStartY = global::mouse_wy;

        if (isMouseOnBar)      
            wnd.mode = Window::Mode::Move;
        if (isMouseOnResizer)  
            wnd.mode = Window::Mode::Resize;
    }

    if (wnd.mode == Window::Move || wnd.mode == Window::Resize)  
    {
        const s32 deltaX = wnd::global::mouse_wx - wnd.dragStartX;
        const s32 deltaY = wnd::global::mouse_wy - wnd.dragStartY;
        wnd.dragStartX = wnd::global::mouse_wx;
        wnd.dragStartY = wnd::global::mouse_wy;

        if (wnd.mode == Window::Move) {
            wnd.rect.x += deltaX;
            wnd.rect.y += deltaY;
        }

        if (wnd.mode == Window::Resize) {
            wnd.rect.w += deltaX;
            wnd.rect.h += deltaY;
            Clamp(wnd.rect.w, wnd.limits.x, wnd.limits.w);
            Clamp(wnd.rect.h, wnd.limits.y, wnd.limits.h);
        }  
    }

    //DRAWING
    DrawRectangle(renderGraph, wnd.rect, BLACK2);
    DrawRectangle(renderGraph, bar, isMouseOnBar ? BLACK3 : BLACK1);
    DrawRectangle(renderGraph, resizer, isMouseOnResizer ? RED : BLACK3);
    DrawTextCentered(renderGraph, wnd.title.data, wnd.title.Length(), bar);
}

//! SPECIFIC

inline void DrawFPS(rendering::RenderGraph& renderGraph, const utils::Rect<f32>& rect = { 0, 0, 48, 20 })
{
    DrawRectangle(renderGraph, rect, Colors::BLACK1);

    char fpsStr [10] {};
    const auto res = std::to_chars(fpsStr, fpsStr + 10, dt::fps);
    const auto len = (idx_t)std::distance(fpsStr, res.ptr);
    DrawTextCentered(renderGraph, fpsStr, len, rect, GREEN);
}

} //ns