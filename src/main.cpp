#include "mini/Window/win_Window.hpp"
#include "mini/Window/win_WindowCallback.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Debug/Console.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Memory/Allocator.hpp"

#include "rpg/Scene/Scene.hpp"
#include "rpg/DeltaTime.hpp"

#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Resources.hpp"

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

    //renderer
    auto vkContext   = vk::CreateContext({ wnd.hInst, wnd.hWnd });
    auto vkResources = vk::CreateResources(vkContext);

    //scenes
    using SceneStack = box::Array<rpg::scene::Scene, 4>;
    auto sceneStack  = mem::ClaimBlock<SceneStack>();
    sceneStack->SetCompleteArray();
    auto& currentScene = (*sceneStack)[0];

    rpg::dt::StartClock();

    while (!app::CheckEvent(EventType::Window_Close) 
        && !app::IsPressed(EventType::Keyboard_Escape))
    {
        wnd::PollEvents();
        rpg::dt::CalcDeltaTime();

        currentScene.Update(rpg::dt::seconds);
        currentScene.Render(rpg::dt::seconds);
    }

    mem::Free();
    vkContext.Destroy();

}//main end