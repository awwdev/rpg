//https://github.com/awwdev

#include "dbg/Console.hpp"
#include "wnd/win_Window.hpp"

#include "com/mem/Allocator.hpp"
#include "com/mem/AllocatorPrint.hpp"

#include "gpu/Vulkan/Renderer.hpp"
#include "app/Scene.hpp"
#include "com/DeltaTime.hpp"
#include "res/Resources.hpp"

#include "res/_Old/CpuResources.hpp"


using namespace rpg;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
    dbg::Console console{};
    com::mem::GlobalAllocate();

    {
        wnd::win_Window  window { hInstance, 800, 600 };
        auto ptrResources     = com::mem::ClaimBlock<res::Resources>();
        ptrResources->Load();

        auto ptrCpuResources  = com::mem::ClaimBlock<res::CpuResources>();
        auto ptrRenderer      = com::mem::ClaimBlock<gpu::vuk::Renderer>(gpu::vuk::WindowHandle{window.hInstance, window.hWnd}, *ptrCpuResources);
        auto ptrGameScenes    = com::mem::ClaimBlock<app::GameScene>();
        ptrGameScenes->Create(*ptrCpuResources);

        while (!wnd::HasEvent<wnd::EventType::Window_Close>() && 
               !wnd::HasEvent<wnd::EventType::ESC, wnd::EventState::Pressed>())
        {
            wnd::UpdateEvents(window.hWnd);
            dt::UpdateFPS();

            if (wnd::glo::window_h != 0 && wnd::glo::window_w != 0)
            {
                ptrGameScenes->Update(rpg::dt::seconds, *ptrCpuResources);
                ptrRenderer->Render(rpg::dt::seconds, *ptrGameScenes, *ptrCpuResources);
            }
        }

        gpu::vuk::VkCheck(vkDeviceWaitIdle(gpu::vuk::g_contextPtr->device));
        com::mem::PrintAllocationHTML();
    }
    
    com::mem::GlobalDeallocate();
    system("pause");

    return 0;
}