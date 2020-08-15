//https://github.com/awwdev

#pragma once
#include "mini/Rendering/RenderGraph.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/Utils/DeltaTime.hpp"
#include "mini/Utils/Algorithms.hpp"
#include "mini/Window/WindowEvents.hpp"
#include "mini/Box/String.hpp"

#include <charconv>
#undef DrawText

namespace mini::ui2 {

//? GLOBAL CONSISTENCY

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

//? STRUCTS

struct Window
{
    static constexpr auto TOP_BAR_H = 20;

    box::String<20>  title;
    utils::Rect<f32> rect {};
    utils::Rect<f32> limits = { rect.w, rect.h, f32max, f32max };

    enum Mode { None, Resize, Move } mode = None;

    f32  line = 0; //offset
    void ResetLine() { line  = TOP_BAR_H + 4; }   
    void NextLine()  { line += LINE_HEIGHT; }   
};

template<class T>
struct Slider
{
    static constexpr f32 BACK_H = 16;
    static constexpr f32 KNOB_SIZE = 12;
    static constexpr f32 PADDING = 2;

    box::String<20> name;
    utils::Rect<float> back { 0, 0, 0, BACK_H };
    bool isDragging = false;
    f32 knobPos = 0;
    f32 wref = 1; //important for window resize

    T min {}, max {}; 
    T GetValue() const { return (knobPos / (back.w - KNOB_SIZE - PADDING * 2)) * (max - min); }
};

template<class T>
struct InputField
{
    static constexpr f32 BACK_H = 16;
    static constexpr f32 KNOB_SIZE = 12;
    static constexpr f32 PADDING = 2;

    box::String<20> name;
    utils::Rect<float> back { 0, 0, 0, BACK_H };
};

//? BASIC

inline void DrawText(
    rendering::RenderGraph& renderGraph, 
    chars_t str,
    const f32 x, const f32 y, 
    const Colors col = Colors::WHITE)
{
    for(idx_t i = 0; i < std::strlen(str); ++i) {
        renderGraph.ui_ubo.AppendData({ 
            .rect         = { x + LETTER_SPACE * i, y, LETTER_SIZE, LETTER_SIZE },
            .colorIndex   = col,
            .textureIndex = (u32)str[i]
        });
    }
}

inline void DrawText(
    rendering::RenderGraph& renderGraph, 
    chars_t str,
    const Window& wnd,  
    const Colors col = Colors::WHITE)
{
    DrawText(renderGraph, str, wnd.rect.x, wnd.rect.y + wnd.line, col);
}

inline void DrawRectangle(rendering::RenderGraph& renderGraph, utils::Rect<f32> rect, const Colors col)
{
    renderGraph.ui_ubo.AppendData({ 
        .rect         = rect,
        .colorIndex   = col,
        .textureIndex = FULL_OPAQUE_NO_TEXTURE
    });
}

//? ADVANCED

inline void DrawTextCentered(
    rendering::RenderGraph& renderGraph,
    chars_t str,
    const utils::Rect<f32>& rect, 
    const Colors col = WHITE)
{
    const auto len = std::strlen(str);
    const auto TOTAL_STR_W = (len + 1) * LETTER_SPACE;
    const auto cx = rect.x + rect.w * 0.5f - TOTAL_STR_W * 0.5f;
    const auto cy = rect.y + rect.h * 0.5f - LETTER_SIZE * 0.5f;

    DrawText(renderGraph, str, cx, cy, col);
}

inline void DrawTextCentered(
    rendering::RenderGraph& renderGraph,
    chars_t str, 
    const Window& wnd, 
    const Colors col = WHITE)
{
    DrawTextCentered(renderGraph, str, { wnd.rect.x, wnd.rect.y + wnd.line, wnd.rect.w, LINE_HEIGHT }, col);
}

//? WINDOW

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
        if (isMouseOnBar)      
            wnd.mode = Window::Mode::Move;
        if (isMouseOnResizer)  
            wnd.mode = Window::Mode::Resize;
    }

    if (wnd.mode == Window::Move || wnd.mode == Window::Resize)  
    {
        if (wnd.mode == Window::Move) {
            wnd.rect.x += global::mouse_dx;
            wnd.rect.y += global::mouse_dy;
        }

        if (wnd.mode == Window::Resize) {
            wnd.rect.w += global::mouse_dx;
            wnd.rect.h += global::mouse_dy;
            Clamp(wnd.rect.w, wnd.limits.x, wnd.limits.w);
            Clamp(wnd.rect.h, wnd.limits.y, wnd.limits.h);
        }  
    }

    //DRAWING
    DrawRectangle(renderGraph, wnd.rect, BLACK2);
    DrawRectangle(renderGraph, bar, isMouseOnBar ? BLACK3 : BLACK1);
    DrawRectangle(renderGraph, resizer, isMouseOnResizer ? RED : BLACK3);
    DrawTextCentered(renderGraph, wnd.title.data, bar);
}

//? SLIDER

template<class T>
inline auto DrawSlider(rendering::RenderGraph& renderGraph, Slider<T>& slider)
{
    using namespace utils;
    using namespace wnd;

    const Rect<f32> knob { 
        slider.back.x + slider.PADDING + slider.knobPos, 
        slider.back.y + slider.PADDING, 
        slider.KNOB_SIZE, slider.KNOB_SIZE 
    };

    //INTERACTION
    bool hasChanged = false;
    const bool isMouseOnKnob = IsPointInsideRect(global::mouse_wx, global::mouse_wy, knob);

    if(HasEvent<Mouse_ButtonLeft, Released>())
        slider.isDragging = false;

    if(isMouseOnKnob && HasEvent<Mouse_ButtonLeft, Pressed>()){
        slider.isDragging = true;
    }

    if (slider.isDragging){
        slider.knobPos += global::mouse_dx;
    }

    if (ClampReturnBool(slider.knobPos, 0.f, slider.back.w - slider.KNOB_SIZE - slider.PADDING * 2)) //padding for both slider and knob
        slider.isDragging = false;

    //DRAW
    DrawRectangle(renderGraph, slider.back, BLACK1);
    DrawRectangle(renderGraph, knob, (isMouseOnKnob || slider.isDragging) ? RED : WHITE);
    DrawTextCentered(renderGraph, slider.name.data, slider.back, GREEN); 

    box::String<30> valueStr; 
    valueStr.Append(slider.GetValue());
    DrawText(renderGraph, valueStr.data, slider.back.x, slider.back.y, RED); 

    return slider.GetValue();
}

template<class T>
inline auto DrawSlider(rendering::RenderGraph& renderGraph, Slider<T>& slider, const Window& wnd)
{
    slider.back.x = wnd.rect.x + slider.PADDING;
    slider.back.y = wnd.rect.y + wnd.line;
    slider.back.w = wnd.rect.w - slider.PADDING * 2;
    //account for window resize
    slider.knobPos *= wnd.rect.w / slider.wref;  //BUG somewhere (scale is not accurate)
    slider.wref = wnd.rect.w;
    return DrawSlider(renderGraph, slider);
}

//? INPUT FIELD

inline void DrawInputField(rendering::RenderGraph& renderGraph)
{

}

//? SPECIFIC

inline void DrawFPS(rendering::RenderGraph& renderGraph, const utils::Rect<f32>& rect = { 0, 0, 48, 20 })
{
    DrawRectangle(renderGraph, rect, Colors::BLACK1);

    char fpsStr [10] {};
    const auto res = std::to_chars(fpsStr, fpsStr + 10, dt::fps);
    DrawTextCentered(renderGraph, fpsStr, rect, GREEN);
}

} //ns