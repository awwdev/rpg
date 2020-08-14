//https://github.com/awwdev

#pragma once
#include "mini/Window/WindowEvents.hpp"

namespace mini::app {
    
namespace global  
{
    enum InputMode { PlayMode, UI_Mode, FlyMode } inputMode = UI_Mode;
    InputMode prev = UI_Mode;
}

inline void UpdateInputMode()
{
    if (wnd::HasEvent<wnd::F1, wnd::Pressed>())
        global::prev = global::inputMode = global::PlayMode;
    
    if (wnd::HasEvent<wnd::F2, wnd::Pressed>())
        global::prev = global::inputMode = global::UI_Mode;   

    if (wnd::HasEvent<wnd::Mouse_ButtonRight, wnd::PressedOrHeld>()) 
        global::inputMode = global::FlyMode;
    if (wnd::HasEvent<wnd::Mouse_ButtonRight, wnd::Released>()) 
        global::inputMode = global::prev;

        
}

}//ns