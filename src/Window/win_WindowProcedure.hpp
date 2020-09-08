//https://github.com/awwdev

#pragma once
#include "Box/Optional.hpp"
#include "Window/WindowEvents.hpp"
#include "App/InputMode.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>

//! important: when asking for specific button state use BIG(!) letters

namespace rpg::wnd {

namespace priv 
{
    inline void ResetEvents(HWND) 
    {
        //"advance" or reset input
        FOR_ARRAY(global::frameEvents, i)
        {
            const auto t = global::frameEvents[i];
            if  (global::events[t] == Pressed) 
                 global::events[t] = Held;
            else global::events[t] = None;   
        }
        global::chars.Clear();
        global::frameEvents.Clear();

        if (global::resizeState == global::ResizeState::Begin)
            global::resizeState  = global::ResizeState::End;
        else 
            global::resizeState  = global::ResizeState::None;

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

inline void WmSize(WPARAM, LPARAM lParam)
{
    global::resizeState = global::ResizeState::Begin;
    global::window_w = LOWORD(lParam);
    global::window_h = HIWORD(lParam);
}

inline void WmMove(WPARAM, LPARAM lParam)
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

inline void WmMouseMove(WPARAM, LPARAM lParam)
{
    AddEvent<Mouse_Move>();
    global::mouse_wx = GET_X_LPARAM(lParam);
    global::mouse_wy = GET_Y_LPARAM(lParam);
}

inline void WmMouseWheel(WPARAM wParam, LPARAM)
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

inline void WmLButtonDown(WPARAM, LPARAM lParam)
{
    AddEvent<Mouse_ButtonLeft, Pressed>();
    global::mouse_wx = GET_X_LPARAM(lParam);
    global::mouse_wy = GET_Y_LPARAM(lParam);
}

inline void WmLButtonUp(WPARAM, LPARAM lParam)
{
    AddEvent<Mouse_ButtonLeft, Released>();
    global::mouse_wx = GET_X_LPARAM(lParam);
    global::mouse_wy = GET_Y_LPARAM(lParam);
}

inline void WmRButtonDown(WPARAM, LPARAM lParam)
{
    AddEvent<Mouse_ButtonRight, Pressed>();
    global::mouse_wx = GET_X_LPARAM(lParam);
    global::mouse_wy = GET_Y_LPARAM(lParam);
}

inline void WmRButtonUp(WPARAM, LPARAM lParam)
{
    AddEvent<Mouse_ButtonRight, Released>();
    global::mouse_wx = GET_X_LPARAM(lParam);
    global::mouse_wy = GET_Y_LPARAM(lParam);
}

inline void WmKeyDown(WPARAM wParam, LPARAM)
{
    global::events[wParam] = Pressed;
    global::frameEvents.Append((EventType)wParam);
}

inline void WmKeyUp(WPARAM wParam, LPARAM)
{
    global::events[wParam] = Released;
    global::frameEvents.Append((EventType)wParam);
}

inline void WmChar(WPARAM wParam, LPARAM)
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
        case WM_RBUTTONDOWN: WmRButtonDown(wParam, lParam);         break;
        case WM_RBUTTONUP:   WmRButtonUp(wParam, lParam);           break;
        case WM_CHAR:        WmChar(wParam, lParam);                break;
        case WM_KEYDOWN:     WmKeyDown(wParam, lParam);             break;
        case WM_KEYUP:       WmKeyUp(wParam, lParam);               break;
        case WM_SETCURSOR:   return WmSetCursor(hWnd, uMsg, wParam, lParam);

        default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

}//NS