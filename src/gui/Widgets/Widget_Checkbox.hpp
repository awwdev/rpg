//https://github.com/awwdev

#pragma once
#include "gui/Widgets/Widget_Window.hpp"

namespace rpg::gui {

struct Widget_Checkbox
{
    com::Rectf rect;
    com::String<30> label;
    bool isChecked = false;

    void Update(gpu::RenderData& renderData, bool const insideWindow = true)
    {
        if (insideWindow == false)
            return;

        const com::Rectf boxOutline { 
            rect.x + rect.width/2,
            rect.y,
            LINE_HEIGHT,
            rect.height
        };

        const bool isMouseOnBox = com::IsPointInsideRect(wnd::glo::mouse_wx, wnd::glo::mouse_wy, boxOutline);
        if (isMouseOnBox && wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Released>()){
            isChecked = !isChecked;
        }

        const float filledPadding = 4;
        const com::Rectf boxFilled { 
            rect.x + rect.width/2 + filledPadding,
            rect.y + filledPadding,
            LINE_HEIGHT - filledPadding * 2,
            rect.height - filledPadding * 2
        };

        AddText(renderData, label, rect.x, rect.y);
        AddRectOutline(renderData, boxOutline, Colors::White);
        if (isChecked)
            AddRect(renderData, boxFilled, Colors::White);

    }

    void Update(gpu::RenderData& renderData, Widget_Window& wnd)
    {
        if (wnd.isClosed)
            return;

        bool const insideWindow = wnd.CalculateRow(rect, LINE_HEIGHT);
        return Update(renderData, insideWindow);
    }

};

}//ns