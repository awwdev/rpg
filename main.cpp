#include "MiniSTL/Window/Window.hpp"

int WINAPI wWinMain(
    _In_        HINSTANCE hInstance,
    _In_opt_    HINSTANCE hPrevInstance,
    _In_        PWSTR pCmdLine,
    _In_        int nCmdShow
)
{
    ///setup
    auto wnd = mini::wnd::mini_CreateWindow(hInstance, 800, 600);

    ///main loop
    while (wnd.isOpen) {

        for (MSG msg; PeekMessage(&msg, NULL, 0, 0, PM_REMOVE); ) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        SwapBuffers(wnd.hDc);
    }

    ///clean-up
    DestroyWindow(wnd.hWnd);
    UnregisterClass(L"className", hInstance);

    return 0;
}