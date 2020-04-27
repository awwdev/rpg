#pragma once
#include "MiniSTL/Window/AppEvents.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef max
#include <windowsx.h>


namespace mini::wnd
{
    using namespace mini::app;

    inline void PollEvents()
    {
        for (MSG msg; PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);) {
            //translate?
            DispatchMessage(&msg);
        }
    }


    static LRESULT WindowCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg) {

        ///mouse

        case WM_LBUTTONDOWN:
            mouseEvents.Append(EvMouse::Pressed_LEft, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;

        case WM_LBUTTONUP:
            mouseEvents.Append(EvMouse::Released_Left, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;

        case WM_RBUTTONDOWN:
            mouseEvents.Append(EvMouse::Pressed_Right, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;

        case WM_RBUTTONUP:
            mouseEvents.Append(EvMouse::Released_Right, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;

        ///keyboard

        case WM_KEYDOWN:
            switch (wParam) {
            case VK_ESCAPE:
                keyboardEvents.Append(EvKeyboard::Pressed_Escape);
                keyboardDown.Set<true>(EvKeyboard::Pressed_Escape);
                break;
            case 'W':
                keyboardEvents.Append(EvKeyboard::Pressed_W);
                keyboardDown.Set<true>(EvKeyboard::Pressed_W);
                break;
            }
            break;

        case WM_KEYUP:
            switch (wParam) {
            case VK_ESCAPE:
                keyboardEvents.Append(EvKeyboard::Released_Escape);
                keyboardDown.Set<false>(EvKeyboard::Pressed_Escape);
                break;
            case 'W':
                keyboardEvents.Append(EvKeyboard::Released_W);
                keyboardDown.Set<false>(EvKeyboard::Pressed_W);
                break;
            }
            break;

        ///window

        case WM_CLOSE:
            windowEvents.Append(wnd::EvWindow::Close);
            break;

            ///
        default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }

        return 0;
    }

}//ns