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

    while (!app::CheckEvent(Event::Type::Window_Close) && !app::IsPressed(Event::Type::Keyboard_Escape))
    {
        wnd::PollEvents();

        if (app::CheckEvent(Event::Type::Keyboard_W, Event::State::Released)) {
            DLOG("released w");
        }

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