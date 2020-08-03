//https://github.com/awwdev

#pragma once
#include "mini/Box/Optional.hpp"
#include "mini/Window/WindowEvents.hpp"
#include "mini/App/Input.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>

//! important: when asking for specific button state use BIG(!) letters

namespace mini::wnd
{
    namespace priv 
    {
        inline void ResetEvents(HWND hWnd) 
        {
            //"advance" or reset input
            FOR_ARRAY(global::frameEvents, i)
            {
                const auto t = global::frameEvents[i];
                if  (global::events[t] == Pressed) global::events[t] = Held;
                else global::events[t] = None;     
            }
            global::chars.Clear();
            global::frameEvents.Clear();

            const auto cx = global::window_x + global::window_w/2;
            const auto cy = global::window_y + global::window_h/2;

            POINT point;
            GetCursorPos(&point);
            global::mouse_dx = (s32)point.x - global::mouse_x; 
            global::mouse_dy = (s32)point.y - global::mouse_y;
            global::mouse_x  = (s32)point.x;
            global::mouse_y  = (s32)point.y;

            if (app::global::inputMode == app::global::FlyMode ||
                app::global::inputMode == app::global::PlayMode) {
                SetCursorPos(cx, cy);
                global::mouse_x = cx;
                global::mouse_y = cy;
            }

        }
    }

    inline void UpdateEvents(HWND hWnd)
    {
        priv::ResetEvents(hWnd);
        for (MSG msg; PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);) { 
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    //? WINDOWS MESSAGES

    inline void WmSize(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam) 
        {
            case SIZE_MAXIMIZED: 
            case SIZE_MINIMIZED: 
                AddEvent<Window_Resize>();
            break;

            case SIZE_RESTORED: //this means spamming
            if (global::frameEvents.Contains(Window_Resize) == nullptr)
                AddEvent<Window_Resize>();
            break;
        }

        global::window_w = LOWORD(lParam);
        global::window_h = HIWORD(lParam);
    }

    inline void WmMove(WPARAM wParam, LPARAM lParam)
    {
        if (!wnd::global::frameEvents.Contains(Window_Move))
            AddEvent<Window_Move>();
        wnd::global::window_x = (s32) LOWORD(lParam); 
        wnd::global::window_y = (s32) HIWORD(lParam);
    }

    inline void WmClose(WPARAM, WPARAM)
    {
        AddEvent<Window_Close>();
    }

    inline void WmMouseMove(WPARAM wParam, LPARAM lParam)
    {
        AddEvent<Mouse_Move>();
        global::mouse_wx = GET_X_LPARAM(lParam);
        global::mouse_wy = GET_Y_LPARAM(lParam);
    }

    inline void WmMouseWheel(WPARAM wParam, LPARAM lParam)
    {
        AddEvent<Mouse_Scroll>();
        global::mouse_scroll_delta = GET_WHEEL_DELTA_WPARAM(wParam);
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
        AddEvent<Mouse_ButtonLeft, Pressed>();
        global::mouse_wx = GET_X_LPARAM(lParam);
        global::mouse_wy = GET_Y_LPARAM(lParam);
    }

    inline void WmLButtonUp(WPARAM wParam, LPARAM lParam)
    {
        AddEvent<Mouse_ButtonLeft, Released>();
        global::mouse_wx = GET_X_LPARAM(lParam);
        global::mouse_wy = GET_Y_LPARAM(lParam);
    }

    inline void WmKeyDown(WPARAM wParam, LPARAM lParam, HWND hWnd)
    {
        global::events[wParam] = Pressed;
        global::frameEvents.Append((EventType)wParam);
    }

    inline void WmKeyUp(WPARAM wParam, LPARAM lParam)
    {
        global::events[wParam] = Released;
        global::frameEvents.Append((EventType)wParam);
    }

    inline void WmChar(WPARAM wParam, LPARAM lParam)
    {
        global::chars.Append((char)wParam);        
    }

    inline LRESULT CALLBACK CustomWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch(uMsg)
        {
            case WM_SIZE:        WmSize (wParam, lParam);               break;
            case WM_CLOSE:       WmClose(wParam, lParam);               break;
            case WM_MOVE:        WmMove(wParam, lParam);                break;
            case WM_MOUSEMOVE:   WmMouseMove(wParam, lParam);           break;
            case WM_MOUSEWHEEL:  WmMouseWheel(wParam, lParam);          break;
            case WM_LBUTTONDOWN: WmLButtonDown(wParam, lParam);         break;
            case WM_LBUTTONUP:   WmLButtonUp(wParam, lParam);           break;
            case WM_CHAR:        WmChar(wParam, lParam);                break;
            case WM_KEYDOWN:     WmKeyDown(wParam, lParam, hWnd);       break;
            case WM_KEYUP:       WmKeyUp(wParam, lParam);               break;
            case WM_SETCURSOR:   return WmSetCursor(hWnd, uMsg, wParam, lParam);

            default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
        return 0;
    }

}//ns