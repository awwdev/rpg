#include "MiniSTL/Window/win_Window.hpp"
#include "MiniSTL/Window/win_WindowCallback.hpp"
#include "MiniSTL/Debug/Logger.hpp"
#include "MiniSTL/MAth/Matrix.hpp"
#include "MiniSTL/Debug/Console.hpp"
#include "MiniSTL/Box/Array.hpp"
using namespace mini;

constexpr box::Bitset<9> b { 0b1'1101'1100 };
constexpr auto bar = b.Test<0>();
constexpr auto num = b.Decimal();
constexpr auto fre = b.FindFirstFreeBit();

int WINAPI wWinMain(
    _In_        HINSTANCE hInstance,
    _In_opt_    HINSTANCE hPrevInstance,
    _In_        PWSTR pCmdLine,
    _In_        int nCmdShow)
{
    const auto con = dbg::SetupConsole();
    const auto wnd = wnd::mini_CreateWindow(hInstance, 800, 600);

    box::Bitset<9> b { 0b1'1111'1111 };
    //b.Set(1000, true);
    DLOG(b.Decimal()); //CURRENT WORK TEST

    struct Foo { float n = 0; 
        bool operator==(const float f) { return n == f; }
    };

    enum class ArrTestE : i16 { A = -1, B = 3, C= 1000 };
    //box2::Array<Foo, ArrTestE::B> arr1{ Foo{32.f}, Foo{12.f} };
    box::Array<float, ArrTestE::B> arr1 { 32.f, 12.f };
    box::Array<float, 4> arr2 { arr1 };
    arr2 = arr1;

   //arr1.Append(1.f);
   //arr1.Append(2.f);
   //arr1.Append(3.f);
   //arr1.RemoveOrdered(0);
   //arr1.Insert(ArrTestE::C, 42.f);

    //DLOG(arr1.Contains(2.f) ? "found" : "not found");

    FOR_ARRAY(arr2, i)
    {
        //DLOG(i, arr1[i]->n);
        DLOG(i, arr2[i]);
    }

    //DLOG(arr1.Last()->n);
    //DLOG(arr1[10]);
    //math::Mat<int, 4, 3> M { 1, 2, 3, 4 };
    //DLOG(math::ToString(M));

    while (!app::CheckEvent(EventType::Window_Close) && !app::IsPressed(EventType::Keyboard_Escape))
    {
        wnd::PollEvents();

        if (app::CheckEvent(EventType::Keyboard_W, EventState::Released)) {
            DLOG("released w");
        }

        //update current scene
        //draw current scene
    }
}

//TODO: same bounds check in bitset as in array!

//variant
//string
//profiling
//print input?
//scene queue