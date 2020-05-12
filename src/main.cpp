#include "MiniSTL/Window/win_Window.hpp"
#include "MiniSTL/Window/win_WindowCallback.hpp"
#include "MiniSTL/Debug/Logger.hpp"
#include "MiniSTL/Debug/Console.hpp"
#include "MiniSTL/Debug/Profiler.hpp"
#include "MiniSTL/Box/String.hpp"
#include "MiniSTL/Memory/Allocator.hpp"

using namespace mini;

int WINAPI wWinMain(
    _In_        HINSTANCE hInstance,
    _In_opt_    HINSTANCE hPrevInstance,
    _In_        PWSTR pCmdLine,
    _In_        int nCmdShow)
{
    const auto con = dbg::CreateConsole();
    const auto wnd = wnd::mini_CreateWindow(hInstance, 800, 600);
    
    mini::mem::Allocate();
    auto o1 = mini::mem::ClaimBlock<mini::box::String<100>>();
    auto o2 = mini::mem::ClaimBlock<mini::box::String<100>>();

    o1->Set("Hello World");
    o2->Set("This is great!");

    LOG(*o1, *o2);


    while (!app::CheckEvent(EventType::Window_Close) && !app::IsPressed(EventType::Keyboard_Escape))
    {
        wnd::PollEvents();
        
    
        if (app::CheckEvent(EventType::Keyboard_W, EventState::Released)) {
            mini::dbg::dlog("released w");
        }

        //update current scene
        //draw current scene
    }
}

//variant
//print input?
//scene queue