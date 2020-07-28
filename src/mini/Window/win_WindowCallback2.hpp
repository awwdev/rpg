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
        FOR_ARRAY(g_tmpBuffer, i)
        {
            const auto t = g_tmpBuffer[i];
            if (g_input[t] == Pressed) g_input[t] = Held;
            else                       g_input[t] = None;     
        }
        g_tmpBuffer.Clear();

        //outside the window mouse movement
        if (wnd2::g_ui_mode == false)
        {
            RECT wndRect;
            GetWindowRect(hWnd, &wndRect);
            const auto cx = wndRect.left + (wndRect.right - wndRect.left)/2;
            const auto cy = wndRect.top  + (wndRect.bottom - wndRect.top)/2;

            POINT point;
            GetCursorPos(&point);
            g_mouse_delta_x = point.x - (cx);
            g_mouse_delta_y = point.y - (cy);

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
                    wnd2::g_ui_mode = !wnd2::g_ui_mode;
                if (!wnd2::g_ui_mode) {
                    RECT wndRect;
                    GetWindowRect(hWnd, &wndRect);
                    const auto cx = wndRect.left + (wndRect.right - wndRect.left)/2;
                    const auto cy = wndRect.top  + (wndRect.bottom - wndRect.top)/2;
                    SetCursorPos(cx, cy);
                }

                g_tmpBuffer.Append((InputType)vKey);
            }

            else if (raw.header.dwType == RIM_TYPEMOUSE) 
            {

            }
        }
    }

    inline void WmSize(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam) 
        {
            case SIZE_MAXIMIZED: 
            case SIZE_MINIMIZED: 
                AddEvent<Window_Resize>();
            break;

            case SIZE_RESTORED: //this means spamming
            if (g_tmpBuffer.Contains(Window_Resize) == nullptr)
                AddEvent<Window_Resize>();
            break;
        }

        wnd::window_w = LOWORD(lParam);
        wnd::window_h = HIWORD(lParam);
        g_window_w = LOWORD(lParam);
        g_window_h = HIWORD(lParam);
    }

    inline void WmClose(WPARAM, WPARAM)
    {
        AddEvent<Window_Close>();
    }

    inline void WmMouseMove(WPARAM wParam, LPARAM lParam)
    {
        AddEvent<Mouse_Move>();
        g_mouse_window_x = GET_X_LPARAM(lParam);
        g_mouse_window_y = GET_Y_LPARAM(lParam);
    }

    inline void WmMouseWheel(WPARAM wParam, LPARAM lParam)
    {
        AddEvent<Mouse_Scroll>();
        g_mouse_scroll_delta = GET_WHEEL_DELTA_WPARAM(wParam);
    }

    inline auto WmSetCursor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        const auto hit = LOWORD(lParam);
        switch(hit)
        {
            case HTCLIENT:  SetCursor(LoadCursor(NULL, IDC_ARROW)); break;
            default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
        return 0;
    }

    inline void WmLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        AddEvent<Mouse_ButtonLeft>(Pressed);
        g_mouse_window_x = GET_X_LPARAM(lParam);
        g_mouse_window_y = GET_Y_LPARAM(lParam);
    }

    inline void WmLButtonUp(WPARAM wParam, LPARAM lParam)
    {
        AddEvent<Mouse_ButtonLeft>(Released);
        g_mouse_window_x = GET_X_LPARAM(lParam);
        g_mouse_window_y = GET_Y_LPARAM(lParam);
    }

    inline LRESULT CALLBACK CustomWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch(uMsg)
        {
            case WM_INPUT:       WmInput(wParam, lParam, hWnd);         break;
            case WM_SIZE:        WmSize (wParam, lParam);               break;
            case WM_CLOSE:       WmClose(wParam, lParam);               break;
            case WM_MOUSEMOVE:   WmMouseMove(wParam, lParam);           break;
            case WM_MOUSEWHEEL:  WmMouseWheel(wParam, lParam);          break;
            case WM_LBUTTONDOWN: WmLButtonDown(wParam, lParam);         break; //TODO: realise with raw input
            case WM_LBUTTONUP:   WmLButtonUp(wParam, lParam);           break;
            case WM_SETCURSOR:   return WmSetCursor(hWnd, uMsg, wParam, lParam);

            default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
        return 0;
    }

}//ns