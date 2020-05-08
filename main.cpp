#include "MiniSTL/Window/win_Window.hpp"
#include "MiniSTL/Window/win_WindowCallback.hpp"
#include "MiniSTL/Debug/Logger.hpp"
#include "MiniSTL/MAth/Matrix.hpp"
#include "MiniSTL/Debug/Console.hpp"
#include "MiniSTL/Box/Array.hpp"
#include "MiniSTL/Box/String.hpp"
#include "MiniSTL/Debug/Profiler.hpp"

using namespace mini;
using namespace mini::box;

struct Foo {
    float f;
    Foo(float x, float y) : f {x} {;}
    Foo(Foo&& other) : f{ other.f } { mini::dbg::dlog("mv"); }
    void operator=(Foo&& other) { f = other.f; mini::dbg::dlog("mv ass"); }
    Foo(const Foo& other) : f{ other.f } { mini::dbg::dlog("cpy"); }
    void operator=(const Foo& other) { f = other.f; mini::dbg::dlog("cpy ass"); }
};

void PtrintFoos(const mini::box::IArray<Foo>& foos)
{
    FOR_ARRAY(foos, i)
    {
        mini::dbg::dlog(foos[i].f);
    }
}

int WINAPI wWinMain(
    _In_        HINSTANCE hInstance,
    _In_opt_    HINSTANCE hPrevInstance,
    _In_        PWSTR pCmdLine,
    _In_        int nCmdShow)
{
    const auto con = dbg::CreateConsole();
    //const auto wnd = wnd::mini_CreateWindow(hInstance, 800, 600);

    mini::box::Array<Foo, 10> arr1 { Foo(1.23, 0), Foo(4.56, 0) };
    arr1.RemoveFast(1);

    PtrintFoos(arr1);
    
    system("pause");    
}




// STRING
//variant
//print input?
//scene queue

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