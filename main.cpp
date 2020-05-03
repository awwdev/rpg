#include "MiniSTL/Window/win_Window.hpp"
#include "MiniSTL/Window/win_WindowCallback.hpp"
#include "MiniSTL/Debug/Logger.hpp"
#include "MiniSTL/MAth/Matrix.hpp"
#include "MiniSTL/Debug/Console.hpp"
#include "MiniSTL/Box/Array.hpp"
using namespace mini;

struct Foo {
    float num;
    //Foo(float n, float x) : num{ n } { ; }
    ~Foo() { DLOG("dtor"); }
};

inline void PrintFooArray(mini::box::IArray<Foo>& foos) {
    FOR_ARRAY(foos, i)
    {
        DLOG(foos[i].num);
    }
    DLOG("---")
}


int WINAPI wWinMain(
    _In_        HINSTANCE hInstance,
    _In_opt_    HINSTANCE hPrevInstance,
    _In_        PWSTR pCmdLine,
    _In_        int nCmdShow)
{
    const auto con = dbg::SetupConsole();

    box::Array<int, 10> arr1;
    arr1.InitCompleteArray();
    box::PrintArray(arr1, "arr1");

    box::Array<Foo, 3> arr2 { Foo{1.f}};
    arr1.AppendReturn(3) = 4;
    arr2.Append(2.f);
    
    box::Array<int, 5u> arr3 = arr1;
    box::Array<Foo, 5u> arr4 = std::move(arr2);

    const auto arrx = std::move(arr1);

    
    PrintFooArray(arr4);
    
    DLOG(arr1.Count());
    DLOG(arr2.Count());

    system("pause");
}




//TODO: perf check std::array and std::vector vs mini::Array
//also profiling refresh



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