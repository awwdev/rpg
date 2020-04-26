#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef max

namespace mini::wnd
{
    struct AppEvent 
    {
        enum Type {
            CLOSE

        } type;

        //todo: own variant class (for details)

        bool operator==(const AppEvent::Type& pType) {
            DLOG("compare");
            return type == pType;
        }
    };


    //a global array is needed because of global wnd procedure
    inline container::Array<wnd::AppEvent, 10> appEvents; 


    inline void PollEvents()
    {
        mini::wnd::appEvents.Clear();
        for (MSG msg; PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);) {
            DispatchMessage(&msg);
        }
    }

}//ns