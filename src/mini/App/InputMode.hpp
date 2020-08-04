//https://github.com/awwdev

#pragma once
#include "mini/Window/WindowEvents.hpp"

namespace mini::app {
    
namespace global  
{
    enum InputMode { PlayMode, UI_Mode, FlyMode } inputMode = FlyMode;
}

inline void UpdateInputMode()
{
    if (wnd::HasEvent<wnd::F1, wnd::Pressed>())
        global::inputMode = global::PlayMode;

    if (wnd::HasEvent<wnd::F2, wnd::Pressed>()) 
        global::inputMode = global::FlyMode;
        
    if (wnd::HasEvent<wnd::F3, wnd::Pressed>())
        global::inputMode = global::UI_Mode;   
                        
}

}//ns