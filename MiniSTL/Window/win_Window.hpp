#pragma once
#include "MiniSTL/Window/win_WindowCallback.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef max
#include <windowsx.h>

namespace mini::wnd
{
    struct Window
    {
        const wchar_t* className = {};
        HWND        hWnd         = NULL;
        HDC         hDc          = NULL;
        HINSTANCE   hInst        = NULL;

        ~Window()
        {
            DestroyWindow(hWnd);
            UnregisterClass(className, hInst);
        }
    };

    inline Window mini_CreateWindow(HINSTANCE hInst, u16 width, u16 height, const wchar_t* className = L"miniClass", const wchar_t* wndName = L"miniWnd")
    {
        WNDCLASSEX wndClass {
            .cbSize         = sizeof(WNDCLASSEX),
            .style          = 0, //CS_HREDRAW | CS_VREDRAW
            .lpfnWndProc    = WindowCallback,
            .cbClsExtra     = 0,
            .cbWndExtra     = 0,
            .hInstance      = hInst,
            .hIcon          = NULL, //LoadIcon
            .hCursor        = NULL, //LoadCursor
            .hbrBackground  = 0, //HBRUSH GetStockObject
            .lpszMenuName   = NULL,
            .lpszClassName  = className,
            .hIconSm        = NULL //LoadImage
        };

        RegisterClassEx(&wndClass);

        const auto hWnd = CreateWindowEx(
            NULL,
            className,
            wndName,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            static_cast<int>(width),
            static_cast<int>(height),
            NULL,
            NULL,
            hInst,
            NULL
        );

        const auto hDc = GetDC(hWnd);
        ShowWindow(hWnd, SW_SHOWDEFAULT);
        //UpdateWindow(window.hWnd);

        return { //avoid that dtor is called in scope!
            .className  = className,
            .hWnd       = hWnd,
            .hDc        = hDc,
            .hInst      = hInst
        };
    }

}//ns