//https://github.com/awwdev

#pragma once
#include "mini/Rendering/RenderGraph.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/Utils/DeltaTime.hpp"
#include "mini/Utils/Algorithms.hpp"
#include "mini/Window/WindowEvents.hpp"
#include "mini/Box/String.hpp"
#include "mini/Debug/Logger.hpp"

#include <charconv>
#include <cstdlib>
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
constexpr f32   LINE_HEIGHT = 18;
constexpr f32   PADDING = 4;

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

template<class T, class = IsArithmetic<T>>
struct Slider
{
    static constexpr f32 BACK_H = 16;

    box::String<20> name;
    utils::Rect<float> back { 0, 0, 0, BACK_H };
    bool isDragging = false;
    f32 knobPos = 0;
    f32 wref = 1; //important for window resize

    T min {}, max {}; 
    T GetValue() const { 
         return (knobPos / (back.w - (back.h - PADDING * 2) - PADDING * 2)) * (max - min); 
         //back.h - PADDING * 2 == knob size
    }
};

template<class T, class = IsArithmetic<T>>
struct InputField
{
    static constexpr f32 BACK_H = 16;
    utils::Rect<float> rect { 0, 0, 0, BACK_H };

    box::String<20> name;
    box::String<20> value;
    bool isActive = false;

    T GetValue() const {
        if (value.Empty()) 
            return {};
        return (T) std::atof(value.data);
    }
};

//TODO: InputField str

//? BASIC

inline void DrawText(
    rendering::RenderGraph& renderGraph, 
    chars_t str, const idx_t len,
    const f32 x, const f32 y, 
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

inline void DrawText(
    rendering::RenderGraph& renderGraph, 
    chars_t str,
    const Window& wnd,  
    const Colors col = Colors::WHITE)
{
    DrawText(renderGraph, str, (idx_t)std::strlen(str), wnd.rect.x, wnd.rect.y + wnd.line, col);
}

template<idx_t N>
inline void DrawText(
    rendering::RenderGraph& renderGraph, 
    const box::String<N>& str,
    Window& wnd,  
    const Colors col = Colors::WHITE)
{
    DrawText(renderGraph, str.data, str.Length(), wnd.rect.x, wnd.rect.y + wnd.line, col);
    wnd.NextLine();
}

template<idx_t N>
inline void DrawText(
    rendering::RenderGraph& renderGraph, 
    const box::String<N>& str,
    const f32 x, const f32 y, 
    const Colors col = Colors::WHITE)
{
    DrawText(renderGraph, str.data, str.Length(), x, y, col);
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
    const auto len = (idx_t)std::strlen(str);
    const auto TOTAL_STR_W = (len + 1) * LETTER_SPACE;
    const auto cx = rect.x + rect.w * 0.5f - TOTAL_STR_W * 0.5f;
    const auto cy = rect.y + rect.h * 0.5f - LETTER_SIZE * 0.5f;

    DrawText(renderGraph, str, len, cx, cy, col);
}

inline void DrawTextCentered(
    rendering::RenderGraph& renderGraph,
    chars_t str, 
    Window& wnd, 
    const Colors col = WHITE)
{
    wnd.NextLine();
    DrawTextCentered(renderGraph, str, { wnd.rect.x, wnd.rect.y + wnd.line, wnd.rect.w, LINE_HEIGHT }, col);
}

//? WINDOW

inline void DrawWindow(rendering::RenderGraph& renderGraph, Window& wnd)
{
    using namespace utils;
    using namespace wnd;

    wnd.ResetLine();

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
        slider.back.x + PADDING + slider.knobPos, 
        slider.back.y + PADDING, 
        slider.back.h - PADDING * 2, 
        slider.back.h - PADDING * 2
    };

    //INTERACTION
    bool hasChanged = false;
    const bool isMouseOnKnob = IsPointInsideRect(global::mouse_wx, global::mouse_wy, knob);

    if(HasEvent<Mouse_ButtonLeft, Released>())
        slider.isDragging = false;

    if(isMouseOnKnob && HasEvent<Mouse_ButtonLeft, Pressed>())
        slider.isDragging = true;

    if (slider.isDragging)
        slider.knobPos += global::mouse_dx;

    if (ClampReturnBool(slider.knobPos, 0.f, slider.back.w - knob.w - PADDING * 2)) //padding for both slider and knob
        slider.isDragging = false;

    //DRAW
    DrawRectangle(renderGraph, slider.back, BLACK1);
    DrawRectangle(renderGraph, knob, (isMouseOnKnob || slider.isDragging) ? GREEN : WHITE);
    DrawTextCentered(renderGraph, slider.name.data, slider.back); 

    box::String<30> valueStr; 
    valueStr.Append(slider.GetValue());
    DrawText(renderGraph, valueStr, slider.back.x, slider.back.y, WHITE); 

    return slider.GetValue();
}

template<class T>
inline auto DrawSlider(rendering::RenderGraph& renderGraph, Slider<T>& slider, Window& wnd)
{
    slider.back.x = wnd.rect.x + PADDING;
    slider.back.y = wnd.rect.y + wnd.line;
    slider.back.w = wnd.rect.w - PADDING * 2;
    //account for window resize
    slider.knobPos *= wnd.rect.w / slider.wref;  //BUG somewhere (scale is not accurate)
    slider.wref = wnd.rect.w;
    wnd.NextLine();
    return DrawSlider(renderGraph, slider);
}

//? INPUT FIELD

template<class T>
inline void DrawInputField(rendering::RenderGraph& renderGraph, InputField<T>& inputField)
{
    using namespace utils;
    using namespace wnd;

    const auto textSize = LETTER_SPACE * inputField.name.Length() + LETTER_SPACE;

    const utils::Rect<f32> back { 
        inputField.rect.x + textSize,
        inputField.rect.y,
        inputField.rect.w - textSize,
        inputField.rect.h
    };

    //INTERACTION
    const bool isMouseOnInput = utils::IsPointInsideRect(global::mouse_wx, global::mouse_wy, back);

    if(isMouseOnInput && HasEvent<wnd::Mouse_ButtonLeft, wnd::Pressed>()){
        inputField.isActive = true;
    }

    if(!isMouseOnInput && HasEvent<wnd::Mouse_ButtonLeft, wnd::Released>()){
        inputField.isActive = false;
    }

    if (inputField.isActive){
        FOR_STRING(global::chars, i) {
            if (global::chars[i] == '\b')
                inputField.value.Pop();
            else
                inputField.value.Append(global::chars[i]);
        }
    }

    //DRAW
    DrawRectangle(renderGraph, back, (isMouseOnInput || inputField.isActive) ? BLACK3 : BLACK1);
    DrawText(renderGraph, inputField.name, inputField.rect.x, inputField.rect.y);
    DrawText(renderGraph, inputField.value, back.x, back.y, GREEN);
}

template<class T>
inline void DrawInputField(rendering::RenderGraph& renderGraph, InputField<T>& inputField, Window& wnd)
{
    inputField.rect.x = wnd.rect.x + PADDING;
    inputField.rect.y = wnd.rect.y + wnd.line;
    inputField.rect.w = wnd.rect.w - PADDING * 2;
    wnd.NextLine();
    DrawInputField(renderGraph, inputField);
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