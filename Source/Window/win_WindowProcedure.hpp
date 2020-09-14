//https://github.com/awwdev

#pragma once
#include "Common/Container/Optional.hpp"
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
        FOR_ARRAY(glo::frameEvents, i)
        {
            const auto t = glo::frameEvents[i];
            if  (glo::events[(idx_t)t] == wnd::EventState::Pressed) 
                 glo::events[(idx_t)t] = EventState::Held;
            else glo::events[(idx_t)t] = EventState::None;   
        }
        glo::chars.Clear();
        glo::frameEvents.Clear();
        glo::mouse_scroll_delta = 0;

        if (glo::resizeState == glo::ResizeState::Begin)
            glo::resizeState  = glo::ResizeState::End;
        else 
            glo::resizeState  = glo::ResizeState::None;

        const auto cx = glo::window_x + glo::window_w/2;
        const auto cy = glo::window_y + glo::window_h/2;

        POINT point;
        GetCursorPos(&point);
        glo::mouse_dx = (s32)point.x - glo::mouse_x; 
        glo::mouse_dy = (s32)point.y - glo::mouse_y;
        glo::mouse_x  = (s32)point.x;
        glo::mouse_y  = (s32)point.y;

        if (app::glo::inputMode == app::glo::FlyMode ||
            app::glo::inputMode == app::glo::PlayMode) {
            SetCursorPos(cx, cy);
            glo::mouse_x = cx;
            glo::mouse_y = cy;
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
    glo::resizeState = glo::ResizeState::Begin;
    glo::window_w = LOWORD(lParam);
    glo::window_h = HIWORD(lParam);
}

inline void WmMove(WPARAM, LPARAM lParam)
{
    if (!wnd::glo::frameEvents.Contains(EventType::Window_Move))
        AddEvent<EventType::Window_Move>();
    wnd::glo::window_x = (s32) LOWORD(lParam); 
    wnd::glo::window_y = (s32) HIWORD(lParam);
}

inline void WmClose(WPARAM, WPARAM)
{
    AddEvent<EventType::Window_Close>();
}

inline void WmMouseMove(WPARAM, LPARAM lParam)
{
    AddEvent<EventType::Mouse_Move>();
    glo::mouse_wx = GET_X_LPARAM(lParam);
    glo::mouse_wy = GET_Y_LPARAM(lParam);
}

inline void WmMouseWheel(WPARAM wParam, LPARAM)
{
    AddEvent<EventType::Mouse_Scroll>();
    glo::mouse_scroll_delta = GET_WHEEL_DELTA_WPARAM(wParam);
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
    AddEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Pressed>();
    glo::mouse_wx = GET_X_LPARAM(lParam);
    glo::mouse_wy = GET_Y_LPARAM(lParam);
}

inline void WmLButtonUp(WPARAM, LPARAM lParam)
{
    AddEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Released>();
    glo::mouse_wx = GET_X_LPARAM(lParam);
    glo::mouse_wy = GET_Y_LPARAM(lParam);
}

inline void WmRButtonDown(WPARAM, LPARAM lParam)
{
    AddEvent<wnd::EventType::Mouse_ButtonRight, wnd::EventState::Pressed>();
    glo::mouse_wx = GET_X_LPARAM(lParam);
    glo::mouse_wy = GET_Y_LPARAM(lParam);
}

inline void WmRButtonUp(WPARAM, LPARAM lParam)
{
    AddEvent<wnd::EventType::Mouse_ButtonRight, wnd::EventState::Released>();
    glo::mouse_wx = GET_X_LPARAM(lParam);
    glo::mouse_wy = GET_Y_LPARAM(lParam);
}

inline void WmKeyDown(WPARAM wParam, LPARAM)
{
    glo::events[wParam] = wnd::EventState::Pressed;
    glo::frameEvents.Append((EventType)wParam);
}

inline void WmKeyUp(WPARAM wParam, LPARAM)
{
    glo::events[wParam] = wnd::EventState::Released;
    glo::frameEvents.Append((EventType)wParam);
}

inline void WmChar(WPARAM wParam, LPARAM)
{
    glo::chars.Append((char)wParam);        
}

inline LRESULT CALLBACK win_CustomWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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