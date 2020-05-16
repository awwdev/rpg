#pragma once
#include "mini/Window/win_WindowCallback.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>
#undef max

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
                .style          = 0, //CS_HREDRAW | CS_VREDRAW
                .lpfnWndProc    = WndProc1,
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

            hWnd = CreateWindowEx(
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

            //const auto hDc = GetDC(hWnd);
            ShowWindow(hWnd, SW_SHOWDEFAULT);
            //UpdateWindow(window.hWnd);
        }

        ~Window()
        {
            DestroyWindow(hWnd);
            UnregisterClass(className, hInstance);
        }
    };

}//ns