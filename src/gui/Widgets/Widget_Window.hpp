//https://github.com/awwdev

#pragma once
#include "gui/GUI_Base.hpp"

namespace rpg::gui {

struct Widget_Window
{
    static constexpr f32 ROW_H = 18;
    static constexpr f32 BAR_H = 20;

    com::String<30> title  { "Window" };
    com::Rectf  rect  { 0, 0, 300, 300 };
    const com::Rectf  limits { 32, 32, f32max, f32max };
    
    enum class Mode
    { 
        None,
        Resize, 
        Move,
    }; 
    Mode mode = Widget_Window::Mode::None;
    bool isClosed = false;
    s32 rowY = 0; //for element arrangement

    void Update(gpu::RenderData& renderData)
    {
        rowY = rect.y + BAR_H + PADDING; //reset (will be increased externally)

        if (mode == Widget_Window::Mode::Move) {
            rect.x += wnd::glo::mouse_dx;
            rect.y += wnd::glo::mouse_dy;
        }

        const com::Rectf bar      { rect.x, rect.y, rect.width, BAR_H };
        const com::Rectf resizer  { rect.x + rect.width -  8, rect.y + rect.height - 8, 8, 8 };
        const com::Rectf closeBtn { rect.x + rect.width - BAR_H, rect.y , BAR_H, BAR_H };

        const bool isMouseOnBar      = com::IsPointInsideRect(wnd::glo::mouse_wx, wnd::glo::mouse_wy, bar);
        const bool isMouseOnResizer  = com::IsPointInsideRect(wnd::glo::mouse_wx, wnd::glo::mouse_wy, resizer);
        const bool isMouseOnCloseBtn = com::IsPointInsideRect(wnd::glo::mouse_wx, wnd::glo::mouse_wy, closeBtn);
        
        if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Released>()) {
            mode = Widget_Window::Mode::None;
        }
        
        if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Pressed>()) {        
            if      (isMouseOnBar)      mode = Widget_Window::Mode::Move; 
            else if (isMouseOnResizer)  mode = Widget_Window::Mode::Resize;
            if      (isMouseOnCloseBtn) isClosed = !isClosed;
        }

        if (mode == Widget_Window::Mode::Resize && !isClosed) {
            rect.width += wnd::glo::mouse_dx;
            rect.height += wnd::glo::mouse_dy;
            com::Clamp(rect.width, limits.x, limits.width);
            com::Clamp(rect.height, limits.y, limits.height);
        }

        AddRect(renderData, bar, isMouseOnBar ? Colors::Black4 : Colors::Black1);
        AddRect(renderData, closeBtn, isMouseOnCloseBtn ? Colors::Red : Colors::Black5);
        AddTextCentered(renderData, title, bar, Colors::Orange);

        if (isClosed)
            return; 

        auto backRect = rect;
        backRect.height -= BAR_H;
        backRect.y += BAR_H;

        AddRect(renderData, backRect, Colors::Black2_Alpha, FULL_OPAQUE_NO_TEXTURE, true);
        AddRect(renderData, resizer, isMouseOnResizer ? Colors::Red : Colors::Black3);
    }

    void CalculateRow(f32& pX, f32& pY)
    {
        pX = rect.x + PADDING;
        pY = rowY;
        rowY += ROW_H;
    }

    void CalculateRow(com::Rectf& pRect, const f32 maxHeight)
    {
        pRect.x = rect.x + PADDING;
        pRect.width = rect.width - PADDING * 2;
        pRect.y = rowY;
        pRect.height = maxHeight;
        com::Clamp(pRect.height, 0, (rect.y + rect.height - PADDING) - pRect.y);
        rowY += pRect.height + PADDING;
    }
};

}//ns