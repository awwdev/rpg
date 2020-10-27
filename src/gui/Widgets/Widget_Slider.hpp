//https://github.com/awwdev

#pragma once
#include "gui/Widgets/Widget_Window.hpp"

namespace rpg::gui {

template<typename T>
struct Widget_Slider
{
    com::Rectf rect;
    com::String<30> label;
    T min, max; //ctor could assert when min == max (would cause nan)
    T value = min;

    bool isDragging; //internal
    f32 dragInitX;   //internal

    T Update(gpu::RenderData& renderData, bool const insideWindow = true)
    {
        if (insideWindow == false)
            return value;

        const com::Rectf back { 
            rect.x + rect.width/2,
            rect.y,
            rect.width/2,
            rect.height
        };

        constexpr f32 KNOB_SIZE = LINE_HEIGHT;
        const f32 knobXNorm = (value - min) / (max - min);
        const f32 knobX = knobXNorm * (back.width - KNOB_SIZE);

        const com::Rectf knob { 
            back.x + 1 + knobX,
            back.y + 1,
            KNOB_SIZE - 2,
            KNOB_SIZE - 2
        };

        const bool isMouseOnKnob = com::IsPointInsideRect(wnd::glo::mouse_window_x, wnd::glo::mouse_window_y, knob);
        if (isMouseOnKnob && wnd::MouseLeftButtonPressed()){
            isDragging = true;
            dragInitX = (f32)wnd::glo::mouse_window_x;
        }
        
        if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Released>())
            isDragging = false;

        if (isDragging){
            const f32 delta = (f32)wnd::glo::mouse_window_x - dragInitX;
            dragInitX = (f32)wnd::glo::mouse_window_x;
            const f32 pxVal = (max-min) / (back.width - KNOB_SIZE);
            value += delta * pxVal;
            com::Clamp(value, min, max);
        }

        AddRect(renderData, back, Colors::Black2_Alpha);
        AddRect(renderData, knob, isMouseOnKnob ? Colors::Black4 : Colors::Black3);
        AddText(renderData, label, rect.x, rect.y);
        com::String<30> valueStr { value };
        AddTextCentered(renderData, valueStr, back);
        return value;
    }

    T Update(gpu::RenderData& renderData, Widget_Window& wnd)
    {
        if (wnd.isClosed)
            return {};

        bool const insideWindow = wnd.CalculateRow(rect, LINE_HEIGHT);
        return Update(renderData, insideWindow);
    }
    
};

}//ns