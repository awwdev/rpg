#pragma once
#include "mini/Window/win_WindowCallback.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>
#include <hidusage.h> 
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
                .style          = 0,
                .lpfnWndProc    = WndProc1,
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
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                static_cast<int>(width),
                static_cast<int>(height),
                NULL,
                NULL,
                hInst,
                NULL
            );

            ShowWindow(hWnd, SW_SHOWDEFAULT);
            SetCursor(LoadCursor(NULL, IDC_ARROW));

            //raw input 
            {
                tagRAWINPUTDEVICE devices [] = 
                {
                    {
                        .usUsagePage    = HID_USAGE_PAGE_GENERIC,
                        .usUsage        = HID_USAGE_GENERIC_MOUSE,
                        .dwFlags        = 0,
                        .hwndTarget     = hWnd,
                    },
                    //{
                    //    .usUsagePage    = HID_USAGE_PAGE_GENERIC,
                    //    .usUsage        = HID_USAGE_GENERIC_KEYBOARD,
                    //    .dwFlags        = 0,
                    //    .hwndTarget     = hWnd,
                    //}
                };
                RegisterRawInputDevices(devices, ARRAY_COUNT(devices), sizeof(tagRAWINPUTDEVICE));
            }
            

        }

        ~Window()
        {
            DestroyWindow(hWnd);
            UnregisterClass(className, hInstance);
        }
    };

}//ns