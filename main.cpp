#include "MiniSTL/Window/win_Window.hpp"
#include "MiniSTL/Window/win_WindowCallback.hpp"
#include "MiniSTL/Debug/Logger.hpp"
#include "MiniSTL/MAth/Matrix.hpp"
#include "MiniSTL/Debug/Console.hpp"
#include "MiniSTL/Box/Array.hpp"
#include "MiniSTL/Debug/Profiler.hpp"

#include <vector>
#include <array>

using namespace mini;


struct Foo {
    float num;
    //Foo(float n, float x) : num{ n } { ; }
    //~Foo() { mini::dbg::dlog("dtor"); }
};

inline void PrintFooArray(mini::box::IArray<Foo>& foos) {
    FOR_ARRAY(foos, i)
    {
        mini::dbg::dlog(foos[i].num);
    }
    mini::dbg::dlog("---");
}


int WINAPI wWinMain(
    _In_        HINSTANCE hInstance,
    _In_opt_    HINSTANCE hPrevInstance,
    _In_        PWSTR pCmdLine,
    _In_        int nCmdShow)
{
    const auto con = dbg::SetupConsole();


    system("pause");
}








//variant
//string
//print input?
//scene queue


/*

const auto wnd = wnd::mini_CreateWindow(hInstance, 800, 600);

while (!app::CheckEvent(EventType::Window_Close) && !app::IsPressed(EventType::Keyboard_Escape))
{
    wnd::PollEvents();

    if (app::CheckEvent(EventType::Keyboard_W, EventState::Released)) {
        DLOG("released w");
    }

    //update current scene
    //draw current scene
}
 
*/