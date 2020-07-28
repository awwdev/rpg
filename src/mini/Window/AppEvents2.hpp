//https://github.com/awwdev

#pragma once
#include "mini/Box/Bitset.hpp"
#include "mini/Box/Array.hpp"

namespace mini::wnd2
{
    enum InputType
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

    enum State : u8
    {
        None        = 0, 
        Pressed     = 1, //once 
        Released    = 2, //once
        Held        = 3,
        Yes         = 4,
    };

    inline State g_input [InputType::ENUM_END];
    inline box::Array<InputType, 10> g_tmpBuffer; //internal usage 
    inline u32 g_window_w, g_window_h;
    inline u32 g_mouse_window_x, g_mouse_window_y;
    inline s32 g_mouse_delta_x, g_mouse_delta_y;
    inline s16 g_mouse_scroll_delta; //windows type short
    inline bool g_ui_mode = true; //TODO: move to UI

    template<auto type>
    bool IsPressed() 
    {
        return g_input[type] == Pressed || g_input[type] == Held;
    }

    template<InputType type>
    void AddEvent(const State state = Yes)
    {
        g_input[type] = state;
        g_tmpBuffer.Append(type);
    }

}//ns