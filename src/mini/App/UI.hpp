//https://github.com/awwdev

#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Utils/Utils.hpp"
#include "mini/App/IRenderer.hpp"
#include "mini/Window/AppEvents.hpp"

namespace mini::app::ui
{
    enum class ButtonState { Idle, Hovered, Hold, Released, Inactive };

    inline ButtonState Button(
        IRenderer& renderer,
        chars_t text, 
        const Rect<int>& rect,
        const math::Vec4f& col = { 1, 1, 1, 1 })
    {
        ButtonState state { ButtonState::Idle };

        if (IsPointInsideRect(wnd::mouse_x, wnd::mouse_y, rect))
        {
            state = ButtonState::Hovered;
            if (wnd::IsPressed(wnd::EventType::Mouse_Left)){
                state = ButtonState::Hold;
            }
            if (wnd::CheckEvent(wnd::EventType::Mouse_Left, wnd::EventState::Released)) {
                state = ButtonState::Released;
            }
        }
        
        switch(state)
        {
            case ButtonState::Idle:     renderer.Add_DrawButton(text, rect, {0.5, 0.5, 0.5, 1}); break;
            case ButtonState::Hovered:  renderer.Add_DrawButton(text, rect, {0.7, 0.7, 0.7, 1}); break;
            case ButtonState::Hold:     renderer.Add_DrawButton(text, rect, {1.0, 1.0, 1.0, 1}); break;
        }

        return state;
    }

}//ns