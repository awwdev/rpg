#include "mini/Window/win_Window.hpp"
#include "mini/Window/win_WindowCallback.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Debug/Console.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Memory/Allocator.hpp"

#include "app/Scene/Scene.hpp"
#include "app/DeltaTime.hpp"

#include "mini/Vulkan/Renderer.hpp"
#include "mini/Resources/ResourceManager.hpp"

using namespace mini;
using namespace mini::wnd;


int WINAPI wWinMain(
    _In_        HINSTANCE hInstance,
    _In_opt_    HINSTANCE hPrevInstance,
    _In_        PWSTR pCmdLine,
    _In_        int nCmdShow)
{
    {
        //? META
        dbg::Console console {};
        wnd::Window  window  { hInstance, 800, 600 };
        mem::GlobalAllocate();
        res::ResourceManager resourceManager;

        //? RENDERER
        auto pRenderer = mem::ClaimBlock<vk::Renderer>(
            vk::WindowHandle{window.hInstance, window.hWnd},
            resourceManager
        );

        //? SCENES
        auto pSceneStack = mem::ClaimBlock<box::Array<app::scene::Scene, 1>>();
        pSceneStack->InitCompleteArray();
        uint32_t sceneIdx = 0;

        //? PROGRAM LOOP
        while (!wnd::CheckEvent(EventType::Window_Close) && !wnd::IsPressed(EventType::Keyboard_Escape))
        {
            wnd::PollEvents();
            app::dt::CalcDeltaTimeFPS();        
            pSceneStack[sceneIdx].Update(app::dt::seconds);
            pRenderer->Render(app::dt::seconds, pSceneStack[sceneIdx]);
        }
        
        //? THE END
        VK_CHECK(vkDeviceWaitIdle(pRenderer->context.device));
    }
    
    mem::GlobalDeallocate();

}//main end