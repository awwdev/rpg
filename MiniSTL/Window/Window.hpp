#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef max

#include "MiniSTL/Debug/Logger.hpp"

inline bool isOpen = false;

inline LRESULT WindowCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

struct Window
{
    WNDCLASSEX  wndClass    = {};
    HWND        hWnd        = NULL;
    HDC         hDc         = NULL;
    bool        isOpen      = false;
};

namespace mini::wnd
{
    inline Window mini_CreateWindow(HINSTANCE hInst, u16 width, u16 height, const wchar_t* className = L"miniClass", const wchar_t* wndName = L"miniWnd")
    {
        Window window;
        
        ///register window class

        window.wndClass = {
            .cbSize         = sizeof(WNDCLASSEX),
            .style          = 0, //CS_HREDRAW | CS_VREDRAW
            .lpfnWndProc    = WindowCallback,
            .cbClsExtra     = 0,
            .cbWndExtra     = 0,
            .hInstance      = hInst,
            .hIcon          = NULL, //LoadIcon
            .hCursor        = NULL, //LoadCursor
            .hbrBackground  = 0, //HBRUSH GetStockObject
            .lpszMenuName   = L"Menu",
            .lpszClassName  = className,
            .hIconSm        = NULL //LoadImage
        };

        RegisterClassEx(&window.wndClass);

        ///create window

        window.hWnd = CreateWindowEx(
            NULL,
            window.wndClass.lpszClassName,
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

        ///show

        window.hDc = GetDC(window.hWnd);
        ShowWindow(window.hWnd, SW_SHOWDEFAULT);
        UpdateWindow(window.hWnd);

        window.isOpen = true;

        return window;
    }

}//ns


inline LRESULT WindowCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {

    case WM_CLOSE:
    {
        //isOpen = false;
        break;
    }
        
    case WM_KEYDOWN:
    {
        if (wParam == VK_ESCAPE) {
            //isOpen = false;
        }
        break;
    }

    default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

//todo: global wnd ptr somehow necessary 
//todo: os agnostic events