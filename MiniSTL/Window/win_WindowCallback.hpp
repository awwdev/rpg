#pragma once
#include "MiniSTL/Window/AppEvents.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef max
#include <windowsx.h>

using namespace mini::app;


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


    static LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg) {

        #define PRESSED(t, ...)  events.Append(t, Event::Pressed, __VA_ARGS__);  pressed.Set<t, true>();
        #define RELEASED(t, ...) events.Append(t, Event::Released, __VA_ARGS__); pressed.Set<t, false>();

        ///mouse

        case WM_LBUTTONDOWN:    PRESSED (Event::Type::Mouse_Left,  GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); break;
        case WM_RBUTTONDOWN:    PRESSED (Event::Type::Mouse_Right, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); break;
        case WM_LBUTTONUP:      RELEASED(Event::Type::Mouse_Left,  GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); break;
        case WM_RBUTTONUP:      RELEASED(Event::Type::Mouse_Right, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); break;

        ///keyboard

        case WM_KEYDOWN:
            switch (wParam) 
            {
                case VK_ESCAPE: PRESSED(Event::Type::Keyboard_Escape); break;
                case 'W':       PRESSED(Event::Type::Keyboard_W); break;
            }
        break;

        case WM_KEYUP:
            switch (wParam) 
            {
                case VK_ESCAPE: RELEASED(Event::Type::Keyboard_Escape); break;
                case 'W':       RELEASED(Event::Type::Keyboard_W); break;
            }
        break;

        ///window

        case WM_CLOSE: events.Append(Event::Type::Window_Close); break;
        
        ///default

        default: return DefWindowProc(hWnd, uMsg, wParam, lParam);

        #undef PRESSED
        #undef RELEASED
        }

        return 0;    
    }

}//ns