#pragma once
#include "mini/Window/AppEvents.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>
#undef max

namespace mini::wnd
{
    inline void PollEvents()
    {
        events.Clear();
        for (MSG msg; PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);) { //probably define a limit per frame
            TranslateMessage(&msg); //virtual to char?
            DispatchMessage(&msg);
        }
    }
    
    static LRESULT __stdcall WndProc1(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg) {

        #define PRESSED(t, ...)  events.Append(t, EventState::Pressed, __VA_ARGS__);  pressed.Set<t, true>();
        #define RELEASED(t, ...) events.Append(t, EventState::Released, __VA_ARGS__); pressed.Set<t, false>();

        ///mouse

        case WM_LBUTTONDOWN:    PRESSED (EventType::Mouse_Left,  GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); break;
        case WM_RBUTTONDOWN:    PRESSED (EventType::Mouse_Right, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); break;
        case WM_LBUTTONUP:      RELEASED(EventType::Mouse_Left,  GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); break;
        case WM_RBUTTONUP:      RELEASED(EventType::Mouse_Right, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); break;

        ///keyboard

        case WM_KEYDOWN:
            switch (wParam) 
            {
                case VK_ESCAPE: PRESSED(EventType::Keyboard_Escape); break;
                case 'W':       PRESSED(EventType::Keyboard_W); break;
                case 'E':       PRESSED(EventType::Keyboard_E); break;
            }
        break;

        case WM_KEYUP:
            switch (wParam) 
            {
                case VK_ESCAPE: RELEASED(EventType::Keyboard_Escape); break;
                case 'W':       RELEASED(EventType::Keyboard_W); break;
                case 'E':       RELEASED(EventType::Keyboard_E); break;
            }
        break;

        ///window

        case WM_CLOSE: events.Append(EventType::Window_Close); break;

        case WM_SIZE: 
        {
            switch (wParam) 
            {
                case SIZE_MAXIMIZED: events.Append(EventType::Window_Resize); break;
                case SIZE_MINIMIZED: events.Append(EventType::Window_Resize); break;
                //case SIZE_RESTORED:  events.Append(EventType::Window_Resize); break;
                //todo issue from max to restored no event created (and restored event will spam on normal move, thanks win api!)
                //so probably some flag is better like IsWindowResizing()
            }
        }
        break;

        case WM_EXITSIZEMOVE: events.Append(EventType::Window_Resize); break; //!todo detail info 
        
        ///default

        default: return DefWindowProc(hWnd, uMsg, wParam, lParam);

        #undef PRESSED
        #undef RELEASED
        }

        return 0;    
    }

}//ns