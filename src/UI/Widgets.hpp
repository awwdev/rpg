//https://github.com/awwdev

#pragma once
#include "Rendering/RenderGraph.hpp"
#include "Utils/Structs.hpp"
#include "Utils/DeltaTime.hpp"
#include "Utils/Algorithms.hpp"
#include "Window/WindowEvents.hpp"
#include "Box/String.hpp"
#include "Debug/Logger.hpp"
#include "App/InputMode.hpp"

//#include <charconv>
//#include <cstdlib>
#undef DrawText

namespace rpg::ui {

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

inline rendering::RenderGraph* g_aciveRenderGraph = nullptr;

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

    void UpdateInputMode()
    {
        if (app::global::inputMode != app::global::Edit_Mode)
            return;

        if (utils::IsPointInsideRect(wnd::global::mouse_wx, wnd::global::mouse_wy, rect))
            app::global::inputMode = app::global::UI_Mode; 
    }
};

template<class T, class = IsArithmetic<T>>
struct Slider
{
    static constexpr f32 BACK_H = 16;

    box::String<100> name {};
    utils::Rect<float> back { 0, 0, 0, BACK_H };
    bool isDragging = false;
    f32 knobPos = 0;
    f32 wref = 1; //important for window resize

    T min {}, max {}; 
    T GetValue() const { 
         return min + (knobPos / (back.w - (back.h - PADDING * 2) - PADDING * 2)) * (max - min); 
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

template<class T, auto MAX_ITEM_COUNT>
struct List
{
    box::String<20> name                    {};
    utils::Rect<f32> rect                   {};
    box::Array<T, MAX_ITEM_COUNT> items     {};
    static constexpr s32 NO_ACTIVE_INDEX  = -1;
    s32 activeIndex                       = NO_ACTIVE_INDEX; 
};

//? BASIC

inline void DrawText(
    chars_t str, const idx_t len,
    const f32 x, const f32 y, 
    const Colors col = Colors::WHITE)
{
    for(idx_t i = 0; i < len; ++i) {
        g_aciveRenderGraph->ui_ubo.AppendData({ 
            .rect           = { x + LETTER_SPACE * i, y, LETTER_SIZE, LETTER_SIZE },
            .colorIndex     = col,
            .textureIndex   = (u32)str[i],
            .padding1       = {},
            .padding2       = {},
        });
    }
}

inline void DrawText(
    chars_t str,
    Window& wnd,  
    const Colors col = Colors::WHITE)
{
    DrawText(str, (idx_t)utils::StrLen(str), wnd.rect.x, wnd.rect.y + wnd.line, col);
    wnd.NextLine();
}

template<idx_t N>
inline void DrawText(
    const box::String<N>& str,
    Window& wnd,  
    const Colors col = Colors::WHITE)
{
    DrawText(str.data, str.Length(), wnd.rect.x, wnd.rect.y + wnd.line, col);
    wnd.NextLine();
}

template<idx_t N>
inline void DrawText(
    const box::String<N>& str,
    const f32 x, const f32 y, 
    const Colors col = Colors::WHITE)
{
    DrawText(str.data, str.Length(), x, y, col);
}

inline void DrawRectangle(utils::Rect<f32> rect, const Colors col)
{
    g_aciveRenderGraph->ui_ubo.AppendData({ 
        .rect           = rect,
        .colorIndex     = col,
        .textureIndex   = FULL_OPAQUE_NO_TEXTURE,
        .padding1       = {},
        .padding2       = {},
    });
}

//? ADVANCED

inline void DrawTextCentered(
    chars_t str,
    const utils::Rect<f32>& rect, 
    const Colors col = WHITE)
{
    const auto len = (idx_t)utils::StrLen(str);
    const auto TOTAL_STR_W = (len + 1) * LETTER_SPACE;
    const auto cx = rect.x + rect.w * 0.5f - TOTAL_STR_W * 0.5f;
    const auto cy = rect.y + rect.h * 0.5f - LETTER_SIZE * 0.5f;

    DrawText(str, len, cx, cy, col);
}

inline void DrawTextCentered(
    chars_t str, 
    Window& wnd, 
    const Colors col = WHITE)
{
    wnd.NextLine();
    DrawTextCentered(str, { wnd.rect.x, wnd.rect.y + wnd.line, wnd.rect.w, LINE_HEIGHT }, col);
}

//? WINDOW

inline void DrawWindow(Window& wnd)
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
    DrawRectangle(wnd.rect, BLACK2);
    DrawRectangle(bar, isMouseOnBar ? BLACK3 : BLACK1);
    DrawRectangle(resizer, isMouseOnResizer ? RED : BLACK3);
    DrawTextCentered(wnd.title.data, bar);
}

//? SLIDER

template<class T>
inline auto DrawSlider(Slider<T>& slider)
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
    //bool hasChanged = false;
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
    DrawRectangle(slider.back, BLACK1);
    DrawRectangle(knob, (isMouseOnKnob || slider.isDragging) ? GREEN : WHITE);
    DrawTextCentered(slider.name.data, slider.back, Colors::BLACK4); 

    box::String<30> valueStr; 
    valueStr.Append(slider.GetValue());
    DrawText(valueStr, slider.back.x, slider.back.y, WHITE); 

    return slider.GetValue();
}

template<class T>
inline auto DrawSlider(Slider<T>& slider, Window& wnd)
{
    slider.back.x = wnd.rect.x + PADDING;
    slider.back.y = wnd.rect.y + wnd.line;
    slider.back.w = wnd.rect.w - PADDING * 2;
    //account for window resize
    slider.knobPos *= wnd.rect.w / slider.wref;  //BUG somewhere (scale is not accurate)
    slider.wref = wnd.rect.w;
    wnd.NextLine();
    return DrawSlider(slider);
}

//? INPUT FIELD

template<class T>
inline auto DrawInputField(InputField<T>& inputField)
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
    DrawRectangle(back, (isMouseOnInput || inputField.isActive) ? BLACK3 : BLACK1);
    DrawText(inputField.name, inputField.rect.x, inputField.rect.y);
    DrawText(inputField.value, back.x, back.y, GREEN);

    return inputField.GetValue();
}

template<class T>
inline auto DrawInputField(InputField<T>& inputField, Window& wnd)
{
    inputField.rect.x = wnd.rect.x + PADDING;
    inputField.rect.y = wnd.rect.y + wnd.line;
    inputField.rect.w = wnd.rect.w - PADDING * 2;
    wnd.NextLine();
    return DrawInputField(inputField);
}

//? LIST

template<class T, auto MAX_ITEM_COUNT>
inline bool DrawList(List<T, MAX_ITEM_COUNT>& list)
{
    using namespace utils;
    using namespace wnd;

    bool hasChanged = false;

    //META
    DrawText(list.name, list.rect.x, list.rect.y);
    const utils::Rect<f32> back{
        list.rect.x,
        list.rect.y + LINE_HEIGHT,
        list.rect.w,
        list.rect.h - LINE_HEIGHT,
    };
    DrawRectangle(back, BLACK1);

    //ITEMS
    FOR_ARRAY(list.items, i){
        const Rect<f32> itemRect {
            back.x + PADDING,
            back.y + i * LINE_HEIGHT + PADDING,
            back.w - PADDING * 2,
            LINE_HEIGHT
        };
        const bool isMouseOnItem = IsPointInsideRect(global::mouse_wx, global::mouse_wy, itemRect);

        if (isMouseOnItem) {
            if (HasEvent<wnd::Mouse_ButtonLeft, wnd::Pressed>()){
                hasChanged = true;
                list.activeIndex = i;
            }
            DrawRectangle(itemRect, BLACK3);
        }

        if ((idx_t)list.activeIndex == i)
            DrawRectangle(itemRect, BLACK4);

        DrawText(list.items[i], itemRect.x, itemRect.y);
    }
    return hasChanged;
}

template<class T, auto MAX_ITEM_COUNT>
inline bool DrawList(List<T, MAX_ITEM_COUNT>& list, Window& wnd)
{
    list.rect.x = wnd.rect.x + PADDING;
    list.rect.w = wnd.rect.w - PADDING * 2;
    list.rect.y = wnd.rect.y + wnd.line;
    wnd.line += list.rect.h;
    return DrawList(list);
}

//? SPECIFIC

inline void DrawFPS(const utils::Rect<f32>& rect = { 0, 0, 48, 20 })
{
    DrawRectangle(rect, Colors::BLACK1);

    char fpsStr [10] {};
    std::to_chars(fpsStr, fpsStr + 10, dt::fps);
    DrawTextCentered(fpsStr, rect, GREEN);
}

} //ns