#include "mini/Window/win_Window.hpp"
#include "mini/Window/win_WindowCallback.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Debug/Console.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Memory/Allocator.hpp"

#include "app/Scene/Scene.hpp"
#include "app/DeltaTime.hpp"

#include "mini/Vulkan/Renderer.hpp"

using namespace mini;
using namespace mini::wnd;


int WINAPI wWinMain(
    _In_        HINSTANCE hInstance,
    _In_opt_    HINSTANCE hPrevInstance,
    _In_        PWSTR pCmdLine,
    _In_        int nCmdShow)
{
    //? META
    dbg::Console console {};
    wnd::Window  window  { hInstance, 800, 600 };
    mem::GlobalAllocate();

    //? RENDERER
    constexpr auto s = sizeof(vk::Renderer);
    auto pRenderer = mem::ClaimBlock<vk::Renderer>(vk::WindowHandle{window.hInstance, window.hWnd});

    //? SCENES
    auto pSceneStack = mem::ClaimBlock<box::Array<app::scene::Scene, 4>>();
    pSceneStack->SetCompleteArray();
    auto& currentScene = (*pSceneStack)[0];

    //? PROGRAM LOOP
    app::dt::StartClock();
    while (!wnd::CheckEvent(EventType::Window_Close) && !wnd::IsPressed(EventType::Keyboard_Escape))
    {
        wnd::PollEvents();
        if (const auto fps = app::dt::CalcDeltaTimeFPS(); fps > 0)
            LOG("fps", fps);

        currentScene.Update(app::dt::seconds);
        pRenderer->Render(app::dt::seconds); //todo: scene data input
    }
    
    //? THE END
    VK_CHECK(vkDeviceWaitIdle(pRenderer->context.device));
    mem::GlobalFree();

}//main end