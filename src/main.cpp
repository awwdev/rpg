#include "mini/Window/win_Window.hpp"
#include "mini/Window/win_WindowCallback.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Debug/Console.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Box/Map.hpp"
#include "mini/Utils/Types.hpp"
#include "mini/Memory/Allocator.hpp"
#include "mini/Memory/AllocatorPrint.hpp"
#include "mini/Vulkan/Renderer.hpp"

#include "mini/App/Scene.hpp"
#include "mini/Utils/DeltaTime.hpp"
#include "mini/Resources/HostResources.hpp"

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
        hostRes::HostResources hostResources {}; //will load immediately for now

        //? RENDERER
        constexpr auto s = sizeof vk::VkRenderer;
        auto ptrRenderer = mem::ClaimBlock<vk::VkRenderer>(
            vk::WindowHandle{window.hInstance, window.hWnd},
            hostResources
        );
        
        //? SCENES
        auto ptrSceneStack = mem::ClaimBlock<box::Array<app::Scene, 1, box::INIT::Yes>>();
        uint32_t sceneIdx = 0;

        //? PROGRAM LOOP
        while (!wnd::CheckEvent(EventType::Window_Close) && !wnd::IsPressed(EventType::Keyboard_Escape))
        {
            wnd::PollEvents();
            dt::UpdateFPS();     

            if (wnd::window_h != 0 && wnd::window_w != 0) {
                ptrRenderer->FrameBegin();
                ptrSceneStack[sceneIdx].Update(ptrRenderer.Get(), mini::dt::seconds);
                ptrRenderer->Render(mini::dt::seconds, ptrSceneStack[sceneIdx]);
            }   
        }
        
        //? THE END
        VK_CHECK(vkDeviceWaitIdle(ptrRenderer->context.device));
    }

    mem::GlobalDeallocate();
    system("pause");

}//main end

//TODO: pixel collision for UI (Button)
//TODO: ecs mockup
//TODO: wireframe mode 
//TODO: events hide internal linkage and pass around (allow to be consumed too)