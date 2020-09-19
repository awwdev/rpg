//https://github.com/awwdev

#pragma once
#include "GUI/GUI_Base.hpp"

namespace rpg::gui {

struct Widget_Window
{
    static constexpr f32 ROW_H = 18;
    static constexpr f32 BAR_H = 20;

    com::String<30> title  { "Window" };
    com::Rect<f32>  rect   { 0, 0, 300, 300 };
    const com::Rect<f32>  limits { 32, 32, f32max, f32max };

    enum class Mode { None, Resize, Move } mode = Widget_Window::Mode::None;
    s32 rowY = 0; //for element arrangement

    void Update(gpu::RenderData& renderData)
    {
        rowY = rect.y + BAR_H + PADDING; //reset (will be increased externally)

        const com::Rect<f32> bar     { rect.x, rect.y, rect.w, BAR_H };
        const com::Rect<f32> resizer { rect.x + rect.w - 8, rect.y + rect.h - 8, 8, 8 };

        const bool isMouseOnBar     = com::IsPointInsideRect(wnd::glo::mouse_wx, wnd::glo::mouse_wy, bar);
        const bool isMouseOnResizer = com::IsPointInsideRect(wnd::glo::mouse_wx, wnd::glo::mouse_wy, resizer);
        
        if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Released>())
            mode = Widget_Window::Mode::None;

        else if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Pressed>()) {        
            if      (isMouseOnBar)     mode = Widget_Window::Mode::Move; 
            else if (isMouseOnResizer) mode = Widget_Window::Mode::Resize;
        }

        if (mode == Widget_Window::Mode::Move) {
            rect.x += wnd::glo::mouse_dx;
            rect.y += wnd::glo::mouse_dy;
        }

        else if (mode == Widget_Window::Mode::Resize) {
            rect.w += wnd::glo::mouse_dx;
            rect.h += wnd::glo::mouse_dy;
            com::Clamp(rect.w, limits.x, limits.w);
            com::Clamp(rect.h, limits.y, limits.h);
        }

        AddRect(renderData, rect, Colors::Black2_Alpha, FULL_OPAQUE_NO_TEXTURE, true);
        AddRect(renderData, bar, isMouseOnBar ? Colors::Black4 : Colors::Black1);
        AddRect(renderData, resizer, isMouseOnResizer ? Colors::Red : Colors::Black3);
        AddTextCentered(renderData, title, bar, Colors::Orange);
    }

    void CalculateRow(f32& pX, f32& pY)
    {
        pX = rect.x + PADDING;
        pY = rowY;
        rowY += ROW_H;
    }

    void CalculateRow(com::Rect<f32>& pRect, const f32 maxHeight)
    {
        pRect.x = rect.x + PADDING;
        pRect.w = rect.w - PADDING * 2;
        pRect.y = rowY;
        pRect.h = maxHeight;
        com::Clamp(pRect.h, 0, (rect.y + rect.h - PADDING) - pRect.y);
        rowY += pRect.h + PADDING;
    }
};

}//ns