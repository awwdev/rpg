//https://github.com/awwdev

#pragma once
#include "UI/Widgets/Widget_Common.hpp"
#include "UI/Widgets/Widget_Window.hpp"

namespace rpg::dbg::gui {

template<class T>
struct Widget_Slider
{
    use::Rect<f32>  rect;
    box::String<30> label;
    T value;
    T min, max;

    bool isDragging;
    f32 dragInitX;
    f32 knobX;

    auto Update(gpu::RenderData& renderData)
    {
        const use::Rect<f32> back { 
            rect.x + rect.w/2,
            rect.y,
            rect.w/2,
            rect.h
        };

        constexpr f32 KNOB_SIZE = LINE_HEIGHT;
        const f32 knobXNorm = value / (max - min);
        const f32 knobX = knobXNorm * (back.w - KNOB_SIZE);

        const use::Rect<f32> knob { 
            back.x + 1 + knobX,
            back.y + 1,
            KNOB_SIZE - 2,
            KNOB_SIZE - 2
        };

        bool isMouseOnKnob = use::IsPointInsideRect(wnd::glo::mouse_wx, wnd::glo::mouse_wy, knob);
        if (isMouseOnKnob && wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Pressed>()){
            isDragging = true;
            dragInitX = (f32)wnd::glo::mouse_wx;
        }
        
        if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Released>())
            isDragging = false;

        if (isDragging){
            const f32 delta = (f32)wnd::glo::mouse_wx - dragInitX;
            dragInitX = (f32)wnd::glo::mouse_wx;
            const f32 pxVal = (max-min) / (back.w - KNOB_SIZE);
            value += delta * pxVal;
            use::Clamp(value, min, max);
        }

        AddRect(renderData, back, Colors::Black2_Alpha);
        AddRect(renderData, knob, isMouseOnKnob ? Colors::Black4 : Colors::Black3);
        AddText(renderData, label, rect.x, rect.y);
        box::String<30> valueStr { value };
        AddTextCentered(renderData, valueStr, back);
        return value;
    }

    auto Update(gpu::RenderData& renderData, Widget_Window& wnd)
    {
        wnd.CalculateRow(rect, LINE_HEIGHT);
        return Update(renderData);
    }

    T GetValue() const
    {
        return (T) std::atof(value.data);
    }
};

}//ns