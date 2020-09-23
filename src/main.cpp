//https://github.com/awwdev

#include "dbg/Console.hpp"
#include "wnd/win_Window.hpp"

#include "com/Memory/Allocator.hpp"
#include "com/Memory/AllocatorPrint.hpp"

#include "gpu/Vulkan/Renderer.hpp"
#include "app/Scene.hpp"
#include "com/DeltaTime.hpp"
#include "res/Resources.hpp"

#include "res/_Old/CpuResources.hpp"


using namespace rpg;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
    dbg::Console console{};
    com::Memory::GlobalAllocate();

    {
        wnd::win_Window  window { hInstance, 800, 600 };
        auto ptrCpuResources = com::Memory::ClaimBlock<res::CpuResources>();
        auto ptrRenderer      = com::Memory::ClaimBlock<gpu::vuk::Renderer>(gpu::vuk::WindowHandle{window.hInstance, window.hWnd}, *ptrCpuResources);
        auto ptrGameScenes    = com::Memory::ClaimBlock<app::GameScene>();
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
        com::Memory::PrintAllocationHTML();
    }
    
    com::Memory::GlobalDeallocate();
    system("pause");

    return 0;
}