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
    
    LRESULT __stdcall WndProc1(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg) {

        #define PRESSED(t, ...)  events.Append(t, EventState::Pressed, __VA_ARGS__);  pressed.Set<t, true>();
        #define RELEASED(t, ...) events.Append(t, EventState::Released, __VA_ARGS__); pressed.Set<t, false>();

        //?mouse

        case WM_LBUTTONDOWN:    PRESSED (EventType::Mouse_Left,  GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); break;
        case WM_RBUTTONDOWN:    PRESSED (EventType::Mouse_Right, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); break;
        case WM_LBUTTONUP:      RELEASED(EventType::Mouse_Left,  GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); break;
        case WM_RBUTTONUP:      RELEASED(EventType::Mouse_Right, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); break;
        
        case WM_MOUSEMOVE:      
        {
            mouse_x = GET_X_LPARAM(lParam); 
            mouse_y = GET_Y_LPARAM(lParam);
        }
        break;             

        //?keyboard

        case WM_KEYDOWN:
            switch (wParam) 
            {
                case VK_ESCAPE: PRESSED(EventType::Keyboard_Escape); break;
            }
        break;
        case WM_CHAR:
            PRESSED(EventType::Keyboard_ASCII);
            events.Last().ascii = (char)wParam;
            asciiPressed = (char)wParam;
        break;

        case WM_KEYUP:
            switch (wParam) 
            {
                case VK_ESCAPE: RELEASED(EventType::Keyboard_Escape); break;
                default:
                {
                    RELEASED(EventType::Keyboard_ASCII);
                    events.Last().ascii = (char)wParam;
                    asciiPressed = 0;
                }
            }
        break;

        //?window

        case WM_CLOSE: events.Append(EventType::Window_Close); break;

        case WM_SIZE: 
        {
            switch (wParam) 
            {
                case SIZE_MAXIMIZED: 
                events.Append(EventType::Window_Resize);
                break;

                case SIZE_MINIMIZED: 
                events.Append(EventType::Window_Resize); 
                break;

                case SIZE_RESTORED: //this means spamming
                if (events.Contains(EventType::Window_Resize) == nullptr)
                    events.Append(EventType::Window_Resize); 
                break;
            }

            window_w = LOWORD(lParam);
            window_h = HIWORD(lParam);
        }
        break;
        
        case WM_SETCURSOR: //prevent cursor redraw
        {
            const auto hit = LOWORD(lParam);
            switch(hit)
            {
                //TODO: more clean way like refresh previous declared cursor?
                case HTCLIENT:  SetCursor(LoadCursor(NULL, IDC_ARROW)); break;
            }
        }
        //!no break (just needed to force arrow on HT CLIENT)

        //?default

        default: return DefWindowProc(hWnd, uMsg, wParam, lParam);

        #undef PRESSED
        #undef RELEASED
        }

        return 0;    
    }

}//ns.