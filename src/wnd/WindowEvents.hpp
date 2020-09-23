//https://github.com/awwdev

#pragma once
#include "com/Containers/Array.hpp"
#include "com/Containers/String.hpp"

namespace rpg::wnd {

enum class EventType
{
    //ascii
    Shift = 16,
    ESC   = 27,

    N0 = 48, N1, N2, N3, N4, N5, N6, N7, N8, N9,
    A  = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    a  = 97, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, 
    F1 = 112, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

    //custom
    Mouse_ButtonLeft,
    Mouse_ButtonRight,
    Mouse_ButtonMiddle,
    Mouse_Move,
    Mouse_Scroll,
    Window_Close,
    Window_Resize,
    Window_Move,

    ENUM_END
};

enum class EventState : u8
{
    None,     
    Pressed,        //once 
    Released,       //once
    Held,    
    PressedOrHeld,
    Set,            //misc like wnd
};

namespace glo
{
    inline EventState events [(idx_t)EventType::ENUM_END];
    inline com::String<10> chars;

    inline s32 window_x, window_y;
    inline u32 window_w, window_h;
    inline s32 mouse_scroll_delta;
    inline s32 mouse_dx, mouse_dy; //delta
    inline u32 mouse_wx, mouse_wy; //window
    inline s32 mouse_x,  mouse_y;  //screen

    enum ResizeState { None, Begin, End } resizeState;

    inline com::Array<EventType, 10> frameEvents; //tmp (internal usage mostly)
}

//abstraction over global access
template<auto type, auto state = EventState::Set> bool HasEvent()
{
         if constexpr (state == EventState::None)            return glo::events[(idx_t)type] == EventState::None;
    else if constexpr (state == EventState::Pressed)         return glo::events[(idx_t)type] == EventState::Pressed;
    else if constexpr (state == EventState::Released)        return glo::events[(idx_t)type] == EventState::Released;
    else if constexpr (state == EventState::Held)            return glo::events[(idx_t)type] == EventState::Held;
    else if constexpr (state == EventState::PressedOrHeld)   return glo::events[(idx_t)type] == EventState::Pressed || glo::events[(idx_t)type] == EventState::Held;
    else if constexpr (state == EventState::Set)             return glo::events[(idx_t)type] == EventState::Set;
}

template<auto TYPE, auto STATE = EventState::Set>
void AddEvent()
{
    glo::events[(idx_t)TYPE] = STATE;
    glo::frameEvents.Append(TYPE);
}

}//NS