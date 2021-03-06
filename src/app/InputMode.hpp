//https://github.com/awwdev

#pragma once
#include "wnd/WindowEvents.hpp"

namespace rpg::app {
    
namespace glo  
{
    enum InputMode { PlayMode, EditMode, UI_Mode, FlyMode };
    InputMode inputMode = EditMode;
    InputMode inputModePrev = inputMode;
}

inline void ResetUpdateInputMode()
{
    //reset and set on UI update
    if (glo::inputMode == glo::UI_Mode)
        glo::inputMode  = glo::EditMode;

    if (wnd::HasEvent<wnd::EventType::F1, wnd::EventState::Pressed>()){
        if (glo::inputMode == glo::PlayMode)
            glo::inputModePrev = glo::inputMode = glo::EditMode;
        else    
            glo::inputModePrev = glo::inputMode = glo::PlayMode;
    }

    if (wnd::HasEvent<wnd::EventType::Mouse_ButtonRight, wnd::EventState::PressedOrHeld>()) 
        glo::inputMode = glo::FlyMode;
    if (wnd::HasEvent<wnd::EventType::Mouse_ButtonRight, wnd::EventState::Released>()) 
        glo::inputMode = glo::inputModePrev;
}

}//ns