#include "MiniSTL/Window/win_Window.hpp"
#include "MiniSTL/Window/win_WindowCallback.hpp"
#include "MiniSTL/Debug/Logger.hpp"
#include "MiniSTL/Math/Matrix.hpp"
#include "MiniSTL/Debug/Console.hpp"
#include "MiniSTL/Box/Array.hpp"
#include "MiniSTL/Box/Bitset.hpp"
#include "MiniSTL/Box/String.hpp"
#include "MiniSTL/Debug/Profiler.hpp"
#include "MiniSTL/Debug/Profiler.hpp"

using namespace mini;


using CARRAY = const char[];
constexpr CARRAY carr = "Hello";
constexpr bool same = std::is_same_v<char, std::decay_t<decltype(std::declval<CARRAY>()[0])>>;

int WINAPI wWinMain(
    _In_        HINSTANCE hInstance,
    _In_opt_    HINSTANCE hPrevInstance,
    _In_        PWSTR pCmdLine,
    _In_        int nCmdShow)
{
    const auto con = dbg::CreateConsole();


    const char x[] = "123";
    const char* y  = "abc";

    mini::box::String<1000> str1 { x };
    mini::box::String<1000> str2 { y };
    str1.Append(str2);

    mini::dbg::dlog(str1);
    mini::dbg::dlog(str2);

    system("pause");





    //mini::dbg::dlog(str.data);

    //const auto wnd = wnd::mini_CreateWindow(hInstance, 800, 600);
    //
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



//variant
//print input?
//scene queue