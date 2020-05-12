#include "MiniSTL/Window/win_Window.hpp"
#include "MiniSTL/Window/win_WindowCallback.hpp"
#include "MiniSTL/Debug/Logger.hpp"
#include "MiniSTL/Debug/Console.hpp"
#include "MiniSTL/Debug/Profiler.hpp"
#include "MiniSTL/Box/String.hpp"
#include "MiniSTL/Memory/Allocator2.hpp"

using namespace mini;

int WINAPI wWinMain(
    _In_        HINSTANCE hInstance,
    _In_opt_    HINSTANCE hPrevInstance,
    _In_        PWSTR pCmdLine,
    _In_        int nCmdShow)
{
    const auto con = dbg::CreateConsole();
    const auto wnd = wnd::mini_CreateWindow(hInstance, 800, 600);
    
    mini::mem2::Allocate();
    mini::mem2::ClaimBlock<mini::box::String<1000>>();
    mini::mem2::ClaimBlock<mini::box::String<1000>>();

    while (!app::CheckEvent(EventType::Window_Close) && !app::IsPressed(EventType::Keyboard_Escape))
    {
        wnd::PollEvents();
        
    
        if (app::CheckEvent(EventType::Keyboard_W, EventState::Released)) {
            mini::dbg::dlog("released w");
            //mini::mem2::PrintMemoryUsage();
        }

        if (app::CheckEvent(EventType::Keyboard_E, EventState::Released)) {
            mini::dbg::dlog("released e");
            mini::mem2::Allocate();
        }
    
        //update current scene
        //draw current scene
    }
}

//variant
//print input?
//scene queue