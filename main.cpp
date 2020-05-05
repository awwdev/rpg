#include "MiniSTL/Window/win_Window.hpp"
#include "MiniSTL/Window/win_WindowCallback.hpp"
#include "MiniSTL/Debug/Logger.hpp"
#include "MiniSTL/MAth/Matrix.hpp"
#include "MiniSTL/Debug/Console.hpp"
#include "MiniSTL/Box/Array.hpp"
#include "MiniSTL/Box/String.hpp"
#include "MiniSTL/Debug/Profiler.hpp"

using namespace mini;

int WINAPI wWinMain(
    _In_        HINSTANCE hInstance,
    _In_opt_    HINSTANCE hPrevInstance,
    _In_        PWSTR pCmdLine,
    _In_        int nCmdShow)
{
    const auto con = dbg::CreateConsole();
    //const auto wnd = wnd::mini_CreateWindow(hInstance, 800, 600);

    mini::box::String<100> str;
    str = "Hello World";
    mini::dbg::dlog(str.data);

    system("pause");

    //while (!app::CheckEvent(EventType::Window_Close) && !app::IsPressed(EventType::Keyboard_Escape))
    //{
    //    wnd::PollEvents();
    //
    //    if (app::CheckEvent(EventType::Keyboard_W, EventState::Released)) {
    //        mini::dbg::dlog("released w");
    //    }
    //
    //    //update current scene
    //    //draw current scene
    //}
}


// STRING
//variant
//print input?
//scene queue