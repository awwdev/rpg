#include "MiniSTL/Window/win_Window.hpp"
#include "MiniSTL/Window/win_WindowCallback.hpp"
#include "MiniSTL/Debug/Logger.hpp"
using namespace mini;

int WINAPI wWinMain(
    _In_        HINSTANCE hInstance,
    _In_opt_    HINSTANCE hPrevInstance,
    _In_        PWSTR pCmdLine,
    _In_        int nCmdShow)
{
    const auto wnd = wnd::mini_CreateWindow(hInstance, 800, 600);

    while (!app::IsPressed(app::EvKeyboard::Pressed_Escape) && !app::CheckEvent(app::EvWindow::Close))
    {
        if (app::CheckEvent(app::EvKeyboard::Pressed_W))  DLOG("w pressed");
        if (app::CheckEvent(app::EvKeyboard::Released_W)) DLOG("w released");
        if (app::IsPressed (app::EvKeyboard::Pressed_W))  DLOG("w held down");

        app::ClearEvents();
        wnd::PollEvents();
        //update current scene
        //draw current scene
    }
}

//todo: variant class
//todo: scene queue (transition)
//todo: string class
//todo: review array once again
//todo: review profiling once again
//todo: review bitset
//todo: wnd callback less writing (find a way)
//todo: print input?