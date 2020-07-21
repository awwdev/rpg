#pragma once
#include "mini/Window/AppEvents.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>
#undef max

namespace mini::wnd
{
    inline void PollEvents(HWND hWnd)
    {
        events.Clear();
        //std::memset(asciiPressed, false, ARRAY_COUNT(asciiPressed));

        for (MSG msg; PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);) { //probably define a limit per frame
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        //outside the window mouse movement
        if (ui_mode == false)
        {
            RECT wndRect;
            GetWindowRect(hWnd, &wndRect);
            const auto cx = wndRect.left + (wndRect.right - wndRect.left)/2;
            const auto cy = wndRect.top  + (wndRect.bottom - wndRect.top)/2;

            POINT point;
            GetCursorPos(&point);
            mouse_dx = point.x - (cx);
            mouse_dy = point.y - (cy);

            SetCursorPos(cx, cy);
        }
    }
    
    LRESULT __stdcall WndProc1(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg) {

        #define PRESSED(t, ...)  events.Append(t, EventState::Pressed, __VA_ARGS__);  pressed.Set<t, true>();
        #define RELEASED(t, ...) events.Append(t, EventState::Released, __VA_ARGS__); pressed.Set<t, false>();

        case WM_MOUSEWHEEL:
        {
            events.Append(EventType::Mouse_Scroll, EventState::None);
            events.Last().scrollDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        }
        break;

        //?keyboard

        case WM_KEYDOWN:
            switch (wParam) 
            {
                case VK_ESCAPE: PRESSED(EventType::Keyboard_Escape); break;
                case VK_F1:    
                {
                    PRESSED(EventType::Keyboard_F1); 
                    ui_mode = !ui_mode; //TODO: move into UI I guess
                    if (!ui_mode) {
                        RECT wndRect;
                        GetWindowRect(hWnd, &wndRect);
                        const auto cx = wndRect.left + (wndRect.right - wndRect.left)/2;
                        const auto cy = wndRect.top  + (wndRect.bottom - wndRect.top)/2;
                        SetCursorPos(cx, cy);
                    }
                } 
                break;
                default:
                {
                    asciiPressed[wParam] = true;
                }
                break;
            }
        break;
        
        case WM_KEYUP:
            switch (wParam) 
            {
                case VK_ESCAPE: RELEASED(EventType::Keyboard_Escape); break;
                case VK_F1:     RELEASED(EventType::Keyboard_F1); break;
                default:
                {
                    asciiPressed[wParam] = false;
                }
            }
        break;

        case WM_CHAR:
        {
            PRESSED(EventType::Keyboard_ASCII);
            events.Last().ascii = wParam;
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
                default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
            }
        }
        break;

        case WM_LBUTTONDOWN: 
        {
            PRESSED (EventType::Mouse_Left, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); 
        }
        break;
        case WM_LBUTTONUP:  
        {
            RELEASED(EventType::Mouse_Left, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }
        break;

        case WM_MOUSEMOVE:
        {
            mouse_client_x = GET_X_LPARAM(lParam);
            mouse_client_y = GET_Y_LPARAM(lParam);
        }
        break;

        case WM_INPUT:
        {
            //TODO: GetRawInputBuffer
            //TODO: multipe keys

            UINT dwSize = 0;
            if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER)) == 0)
            {
                RAWINPUT raw {};
                GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &raw, &dwSize, sizeof(RAWINPUTHEADER));

                if (raw.header.dwType == RIM_TYPEMOUSE) 
                {
                    
                    //mouse_dx = (s32)raw.data.mouse.lLastX;
                    //mouse_dy = (s32)raw.data.mouse.lLastY;
                    //LOG(mouse_dx, mouse_dy);
                    //SetCursorPos(300 + window_w/2.f, 300 + window_h/2.f);
                }
            }

        }
        break;

        //?default

        default: return DefWindowProc(hWnd, uMsg, wParam, lParam);

        #undef PRESSED
        #undef RELEASED
        }

        return 0;    
    }

}//ns