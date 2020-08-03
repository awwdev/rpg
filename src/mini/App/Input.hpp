//https://github.com/awwdev

#pragma once
#include "mini/Window/WindowEvents.hpp"

namespace mini::app
{
    namespace global  
    {
        enum InputMode { PlayMode, UI_Mode, FlyMode } inputMode;
    }

    inline void UpdateInputMode()
    {
        if (wnd::HasEvent<wnd::F1, wnd::Pressed>())
            global::inputMode = global::PlayMode;

        if (wnd::HasEvent<wnd::F2, wnd::Pressed>())
        {
            global::inputMode = global::FlyMode;
            wnd::global::mouse_dx = 0; //reset
            wnd::global::mouse_dy = 0;
        }
            

        if (wnd::HasEvent<wnd::F3, wnd::Pressed>())
        {
            global::inputMode = global::UI_Mode;   
            
        }
                 
   }

}//ns