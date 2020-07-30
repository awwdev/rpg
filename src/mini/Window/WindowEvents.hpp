//https://github.com/awwdev

#pragma once
#include "mini/Box/Bitset.hpp"
#include "mini/Box/Array2.hpp"

namespace mini::wnd
{
    enum EventType
    {
        //ascii
        ESC = 27,
        A   = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        a   = 97, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, 
        F1  = 112, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

        //custom
        Mouse_ButtonLeft,
        Mouse_ButtonRight,
        Mouse_ButtonMiddle,
        Mouse_Move,
        Mouse_Scroll,
        Window_Close,
        Window_Resize,

        ENUM_END
    };

    enum EventState : u8
    {
        None,     
        Pressed,        //once 
        Released,       //once
        Held,    
        PressedOrHeld,
        Set,            //misc like wnd
    };

    namespace global
    {
        inline EventState events [EventType::ENUM_END];
        inline box::String<10> chars;

        inline u32 window_w, window_h;
        inline u32 mouse_wx, mouse_wy; //window
        inline s32 mouse_dx, mouse_dy; //delta
        inline s32 mouse_scroll_delta;

        inline bool ui_debug_mode = true;

        inline box2::Array<EventType, 10> frameEvents; //tmp (internal usage mostly)
    }

    //abstraction over global access
    template<auto type, auto state> bool HasEvent()
    {
             if constexpr (state == EventState::None)            return global::events[type] == None;
        else if constexpr (state == EventState::Pressed)         return global::events[type] == Pressed;
        else if constexpr (state == EventState::Released)        return global::events[type] == Released;
        else if constexpr (state == EventState::Held)            return global::events[type] == Held;
        else if constexpr (state == EventState::PressedOrHeld)   return global::events[type] == Pressed || global::events[type] == Held;
        else if constexpr (state == EventState::Set)             return global::events[type] == Set;
    }

    template<auto TYPE, auto STATE = Set>
    void AddEvent()
    {
        global::events[TYPE] = STATE;
        global::frameEvents.Append(TYPE);
    }

}//ns