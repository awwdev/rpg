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
#include "mini/Vulkan/Rendering.hpp"

using namespace mini;


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
    auto pContext   = mem::ClaimBlock<vk::Context>(vk::WindowHandle{window.hInstance, window.hWnd});
    auto pResources = mem::ClaimBlock<vk::Resources>(pContext.Get());

    //? SCENES
    auto pSceneStack = mem::ClaimBlock<box::Array<rpg::scene::Scene, 4>>();
    pSceneStack->SetCompleteArray();
    auto& currentScene = (*pSceneStack)[0];

    //? PROGRAM LOOP
    rpg::dt::StartClock();
    while (!app::CheckEvent(EventType::Window_Close) && !app::IsPressed(EventType::Keyboard_Escape))
    {
        wnd::PollEvents();
        if (const auto fps = rpg::dt::CalcDeltaTimeFPS(); fps > 0)
            LOG("fps", fps);

        currentScene.Update(rpg::dt::seconds);
        currentScene.Render(rpg::dt::seconds); 

        vk::Render(pContext.Get(), pResources.Get(), rpg::dt::seconds); //todo: inject scene data in some way
    }
    
    VK_CHECK(vkDeviceWaitIdle(pContext->device));
    mem::GlobalFree();

}//main end