//https://github.com/awwdev

#pragma once
#include "wnd/WindowEvents.hpp"

namespace rpg::app {
    
namespace glo  
{
    enum InputMode { PlayMode, Edit_Mode, UI_Mode, FlyMode } inputMode = Edit_Mode;
    InputMode prev = Edit_Mode;
}

inline void ResetUpdateInputMode()
{
    //reset and set on UI update
    if (glo::inputMode == glo::UI_Mode)
        glo::inputMode = glo::Edit_Mode;

    if (wnd::HasEvent<wnd::EventType::F1, wnd::EventState::Pressed>()){
        if (glo::inputMode == glo::PlayMode)
            glo::prev = glo::inputMode = glo::Edit_Mode;
        else    
            glo::prev = glo::inputMode = glo::PlayMode;
    }

    if (wnd::HasEvent<wnd::EventType::Mouse_ButtonRight, wnd::EventState::PressedOrHeld>()) 
        glo::inputMode = glo::FlyMode;
    if (wnd::HasEvent<wnd::EventType::Mouse_ButtonRight, wnd::EventState::Released>()) 
        glo::inputMode = glo::prev;
}

}//ns