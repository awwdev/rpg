//https://github.com/awwdev

#pragma once
#include "GUI/Widgets/Widget_Window.hpp"

namespace rpg::gui {

template<class T>
struct Widget_InputField
{
    com::Rect<f32>  rect;
    com::String<30> label;
    com::String<30> value;
    bool isActive = false;

    auto Update(gpu::RenderData& renderData)
    {
        const com::Rect<f32> back { 
            rect.x + rect.w/2,
            rect.y,
            rect.w/2,
            rect.h
        };

        const bool isMouseOnInput = com::IsPointInsideRect(wnd::glo::mouse_wx, wnd::glo::mouse_wy, back);
        if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Pressed>()){
            isActive = isMouseOnInput;
        }

        if (isActive)
        {
            FOR_STRING(wnd::glo::chars, i) {

                if (wnd::glo::chars[i] == '\b'){
                    value.Pop();
                    continue;
                }

                if (value.Full() == false)
                    value.Append(wnd::glo::chars[i]);   
            }
        }

        AddText(renderData, label, rect.x, rect.y);
        AddRect(renderData, back, Colors::Black2_Alpha);
        if (isActive)
            AddRectOutline(renderData, back, Colors::Black3);
        AddText(renderData, value, back.x, back.y);

        return GetValue();
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