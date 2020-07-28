//https://github.com/awwdev

#pragma once
#include "mini/Window/AppEvents2.hpp"
#include "mini/Box/Optional.hpp"
#include "mini/Window/AppEvents.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace mini::wnd2
{
    inline void PollEvents(HWND hWnd)
    {
        //"advance" or reset input
        FOR_ARRAY(tmpBuffer, i)
        {
            if (tmpBuffer[i] == Pressed) g_input[i] = Held;
            else                         g_input[i] = None;     
        }
        tmpBuffer.Clear();

        //outside the window mouse movement
        if (wnd::ui_mode == false)
        {
            RECT wndRect;
            GetWindowRect(hWnd, &wndRect);
            const auto cx = wndRect.left + (wndRect.right - wndRect.left)/2;
            const auto cy = wndRect.top  + (wndRect.bottom - wndRect.top)/2;

            POINT point;
            GetCursorPos(&point);
            wnd::mouse_dx = point.x - (cx);
            wnd::mouse_dy = point.y - (cy);

            SetCursorPos(cx, cy);
        }

        for (MSG msg; PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);) { 
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    inline void WmInput(WPARAM wParam, LPARAM lParam, HWND hWnd)
    {
        UINT size;
        if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER)) == NULL)
        {
            RAWINPUT raw {}; //assumption: sizeof(raw) >= size
            GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &raw, &size, sizeof(RAWINPUTHEADER));

            if (raw.header.dwType == RIM_TYPEKEYBOARD) 
            {
                const bool isKeyDown = raw.data.keyboard.Flags == RI_KEY_MAKE;
                const auto vKey      = raw.data.keyboard.VKey;

                g_input[vKey] = isKeyDown ? Pressed : Released;
                
                //TODO: place in UI
                if (IsPressed<F1>())
                    wnd::ui_mode = !wnd::ui_mode;
                if (!wnd::ui_mode) {
                    RECT wndRect;
                    GetWindowRect(hWnd, &wndRect);
                    const auto cx = wndRect.left + (wndRect.right - wndRect.left)/2;
                    const auto cy = wndRect.top  + (wndRect.bottom - wndRect.top)/2;
                    SetCursorPos(cx, cy);
                }

                tmpBuffer.Append((InputType)vKey);
            }

            else if (raw.header.dwType == RIM_TYPEMOUSE) 
            {

            }
        }
    }

    inline void WmSize(WPARAM wParam, LPARAM lParam)
    {
        wnd::window_w = LOWORD(lParam);
        wnd::window_h = HIWORD(lParam);

        g_input[Window_Resize] = Yes;
        g_window_w = LOWORD(lParam);
        g_window_h = HIWORD(lParam);
    }

    inline void WmClose(WPARAM, WPARAM)
    {
        g_input[Window_Close] = Yes;
    }

    inline void WmMouseMove(WPARAM wParam, LPARAM lParam)
    {
        wnd::mouse_client_x = GET_X_LPARAM(lParam);
        wnd::mouse_client_y = GET_Y_LPARAM(lParam);
        g_mouse_window_x = GET_X_LPARAM(lParam);
        g_mouse_window_y = GET_Y_LPARAM(lParam);
    }

    inline LRESULT CALLBACK CustomWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch(uMsg)
        {
            case WM_INPUT:      WmInput(wParam, lParam, hWnd); break;
            case WM_SIZE:       WmSize (wParam, lParam);       break;
            case WM_CLOSE:      WmClose(wParam, lParam);       break;
            case WM_MOUSEMOVE:  WmMouseMove(wParam, lParam);   break;

            default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
        return 0;
    }

}//ns