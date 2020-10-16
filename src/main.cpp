//https://github.com/awwdev

#include "dbg/Console.hpp"
#include "wnd/win_Window.hpp"

#include "com/mem/Allocator.hpp"
#include "com/mem/AllocatorPrint.hpp"

#include "gpu/Vulkan/Renderer.hpp"
#include "app/Scene.hpp"
#include "com/DeltaTime.hpp"
#include "res/Resources.hpp"

using namespace rpg;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
    dbg::Console console{};
    com::mem::GlobalAllocate();

    {
        wnd::win_Window  window { hInstance, 800, 600 };
        auto ptrResources  = com::mem::ClaimBlock<res::Resources>();
        auto ptrRenderer   = com::mem::ClaimBlock<gpu::vuk::Renderer>(gpu::vuk::WindowHandle{window.hInstance, window.hWnd}, *ptrResources);
        auto ptrGameScenes = com::mem::ClaimBlock<app::GameScene>(*ptrResources);

        while (!wnd::HasEvent<wnd::EventType::Window_Close>() && 
               !wnd::HasEvent<wnd::EventType::ESC, wnd::EventState::Pressed>())
        {
            wnd::UpdateEvents(window.hWnd);
            dt::UpdateFPS();

            if (wnd::glo::window_h != 0 && wnd::glo::window_w != 0)
            {
                ptrGameScenes->Update(rpg::dt::seconds, *ptrResources);
                ptrRenderer->Render  (rpg::dt::seconds, *ptrGameScenes, *ptrResources);
            }
        }

        gpu::vuk::VkCheck(vkDeviceWaitIdle(gpu::vuk::g_contextPtr->device));
        com::mem::PrintAllocationHTML();
    }
    
    com::mem::GlobalDeallocate();
    system("pause");

    return 0;
}