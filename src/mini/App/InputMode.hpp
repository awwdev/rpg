//https://github.com/awwdev

#pragma once
#include "mini/Window/WindowEvents.hpp"

namespace mini::app {
    
namespace global  
{
    enum InputMode { PlayMode, Edit_Mode, UI_Mode, FlyMode } inputMode = Edit_Mode;
    InputMode prev = Edit_Mode;
}

inline void ResetUpdateInputMode()
{
    //reset and set on UI update
    if (global::inputMode == global::UI_Mode)
        global::inputMode = global::Edit_Mode;

    if (wnd::HasEvent<wnd::F1, wnd::Pressed>()){
        if (global::inputMode == global::PlayMode)
            global::prev = global::inputMode = global::Edit_Mode;
        else    
            global::prev = global::inputMode = global::PlayMode;
    }

    if (wnd::HasEvent<wnd::Mouse_ButtonRight, wnd::PressedOrHeld>()) 
        global::inputMode = global::FlyMode;
    if (wnd::HasEvent<wnd::Mouse_ButtonRight, wnd::Released>()) 
        global::inputMode = global::prev;
}

}//ns