//https://github.com/awwdev

#pragma once
#include "UI/Widgets/Widget_Common.hpp"
#include "UI/Widgets/Widget_Window.hpp"

namespace rpg::dbg::gui {

template<class T>
struct Widget_InputField
{
    use::Rect<f32>  rect;
    box::String<30> label;
    box::String<30> value;
    bool isActive = false;

    auto Update(gpu::RenderData& renderData)
    {
        const use::Rect<f32> back { 
            rect.x + rect.w/2,
            rect.y,
            rect.w/2,
            rect.h
        };

        const bool isMouseOnInput = use::IsPointInsideRect(wnd::glo::mouse_wx, wnd::glo::mouse_wy, back);
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