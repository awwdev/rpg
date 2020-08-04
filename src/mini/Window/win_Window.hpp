#pragma once
#include "mini/Window/win_WindowCallback.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace mini::wnd
{
    struct Window
    {
        const wchar_t* className;
        HWND hWnd;
        HINSTANCE hInstance;

        Window(HINSTANCE hInst, u16 width, u16 height, const wchar_t* pClassName = L"miniClass", const wchar_t* wndName = L"miniWnd")
            : className { pClassName }
            , hInstance { hInst }
        {
            WNDCLASSEX wndClass {
                .cbSize         = sizeof(WNDCLASSEX),
                .style          = 0,
                .lpfnWndProc    = wnd::CustomWindowProc,
                .cbClsExtra     = 0,
                .cbWndExtra     = 0,
                .hInstance      = hInst,
                .hIcon          = NULL,
                .hCursor        = NULL,
                .hbrBackground  = 0,
                .lpszMenuName   = NULL,
                .lpszClassName  = className,
                .hIconSm        = NULL
            };

            RegisterClassEx(&wndClass);

            hWnd = CreateWindowEx(
                NULL,
                className,
                wndName,
                WS_OVERLAPPEDWINDOW,
                300, //wnd x
                300, //wnd y
                static_cast<int>(width),
                static_cast<int>(height),
                NULL,
                NULL,
                hInst,
                NULL
            );

            ShowWindow(hWnd, SW_SHOWDEFAULT);
            //this probably trigger window size message, which sets global window size
            SetCursor(LoadCursor(NULL, IDC_ARROW));

            //make mouse delta 0 at the beginning
            RECT wndRect;
            GetWindowRect(hWnd, &wndRect);
            const auto cx = wndRect.left + (wndRect.right - wndRect.left)/2;
            const auto cy = wndRect.top  + (wndRect.bottom - wndRect.top)/2;
            SetCursorPos(cx, cy);
            wnd::global::mouse_dx = wnd::global::mouse_dy = 0;

            POINT point;
            GetCursorPos(&point);
            wnd::global::mouse_x = point.x;
            wnd::global::mouse_y = point.y;

            wnd::global::window_x = (s32)wndRect.left;
            wnd::global::window_y = (s32)wndRect.top;
            //window size and window screen size are not same
        }

        ~Window()
        {
            DestroyWindow(hWnd);
            UnregisterClass(className, hInstance);
        }
    };

}//ns