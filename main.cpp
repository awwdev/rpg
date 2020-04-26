#include "MiniSTL/Window/Window.hpp"
#include "MiniSTL/Container/Array.hpp"

int WINAPI wWinMain(
    _In_        HINSTANCE hInstance,
    _In_opt_    HINSTANCE hPrevInstance,
    _In_        PWSTR pCmdLine,
    _In_        int nCmdShow)
{
    auto wnd = mini::wnd::mini_CreateWindow(hInstance, 800, 600);

    while (!mini::wnd::appEvents.Contains(mini::wnd::AppEvent::CLOSE)) 
    {
        mini::wnd::PollEvents();
        //update current scene
        //draw current scene
    }

    DestroyWindow(wnd.hWnd);
    UnregisterClass(L"className", hInstance);
}

//todo: variant class
//todo: scene queue (transition)
//todo: string class
//todo: review array once again
//todo: review profiling once again