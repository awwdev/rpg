#include "mini/Window/win_Window.hpp"
#include "mini/Window/win_WindowCallback.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Debug/Console.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Memory/Allocator.hpp"

#include "rpg/Scene/Scene.hpp"
#include "rpg/DeltaTime.hpp"

#include "mini/Vulkan/Context.hpp"

using namespace mini;

int WINAPI wWinMain(
    _In_        HINSTANCE hInstance,
    _In_opt_    HINSTANCE hPrevInstance,
    _In_        PWSTR pCmdLine,
    _In_        int nCmdShow)
{
    //meta
    const auto con = dbg::CreateConsole();
    const auto wnd = wnd::mini_CreateWindow(hInstance, 800, 600);
    mem::Allocate();

    auto context = vk::CreateContext({wnd.hInst, wnd.hWnd});

    //scenes
    using SceneStack = mini::box::Array<rpg::scene::Scene, 4>;
    auto sceneStack  = mini::mem::ClaimBlock<SceneStack>();
    sceneStack->SetCompleteArray();
    auto& currentScene = (*sceneStack)[0];

    rpg::dt::StartClock();

    while (!app::CheckEvent(EventType::Window_Close) 
        && !app::IsPressed(EventType::Keyboard_Escape))
    {
        wnd::PollEvents();
        rpg::dt::CalcDelta();

        currentScene.Update(rpg::dt::seconds);
        currentScene.Render(rpg::dt::seconds);
    }

    mem::Free();

}//main end