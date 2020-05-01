#include "MiniSTL/Window/win_Window.hpp"
#include "MiniSTL/Window/win_WindowCallback.hpp"
#include "MiniSTL/Debug/Logger.hpp"
#include "MiniSTL/MAth/Matrix.hpp"
#include "MiniSTL/Debug/Console.hpp"
#include "MiniSTL/Box/Array2.hpp"
using namespace mini;

constexpr box::Bitset<9> b { 0b1'1101'1100 };
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
    DLOG(b.Decimal()); //CURRENT WORK TEST

    struct Foo { float n = 0; 
        bool operator==(const float f) { return n == f; }
    };

    enum class ArrTestE { A, B = 3 };
    box2::Array<Foo, ArrTestE::B> arr1;

    arr1.Append(1.f);
    arr1.Append(2.f);
    arr1.Append(3.f);
    arr1.Remove(0);

    DLOG(arr1.Contains(2.f) ? "found" : "not found");

    FOR_ARRAY(arr1, i)
    {
        DLOG(i, arr1[i].n);
    }

    //math::Mat<int, 4, 3> M { 1, 2, 3, 4 };
    //DLOG(math::ToString(M));
    //TODO: STRING CLASS

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

//todo: variant class
//todo: scene queue (transition)
//todo: string class
//todo: review array once again
//todo: review profiling once again
//todo: review bitset
//todo: wnd callback less writing (find a way)
//todo: print input?