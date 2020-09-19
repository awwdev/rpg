//https://github.com/awwdev

#include "Window/win_Window.hpp"
#include "Window/win_WindowProcedure.hpp"
#include "Debug/Logger.hpp"
#include "Debug/Console.hpp"
#include "Common/Container/Array.hpp"
#include "Common/Container/EnumMap.hpp"
#include "Common/Types.hpp"

#include "Common/Memory/Allocator.hpp"
#include "Common/Memory/AllocatorPrint.hpp"

#include "GPU/Vulkan/Renderer.hpp"

#include "App/Scene.hpp"
#include "Common/DeltaTime.hpp"
#include "Resources/CpuResources.hpp"

#include "Common/ThreadPool.hpp"

using namespace rpg;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
    dbg::Console console{};

    {
        struct TaskArgsTest { int num; };
        com::ThreadPool<4, TaskArgsTest> threadPool;
        for(int i = 0; i < 10; ++i){
            const auto a = 1;
            const auto b = 2;
            const auto c = 3;
            const auto d = 4;
            threadPool.AssignTask(0, [=]() { std::cout << a; });
            threadPool.AssignTask(1, [=]() { std::cout << b; });
            threadPool.AssignTask(2, [=]() { std::cout << c; });
            threadPool.AssignTask(3, [=]() { std::cout << d; });
            threadPool.WaitForAllFinished();
        }
    }

    system("pause");
    std::exit(0);


    com::mem::GlobalAllocate();

    {
        wnd::win_Window  window { hInstance, 800, 600 };
        auto ptrHostResources = com::mem::ClaimBlock<res::HostResources>();
        auto ptrRenderer      = com::mem::ClaimBlock<gpu::vuk::Renderer>(gpu::vuk::WindowHandle{window.hInstance, window.hWnd}, *ptrHostResources);
        auto ptrGameScenes    = com::mem::ClaimBlock<app::GameScene>();
        ptrGameScenes->Create(*ptrHostResources);

        while (!wnd::HasEvent<wnd::EventType::Window_Close>() && 
               !wnd::HasEvent<wnd::EventType::ESC, wnd::EventState::Pressed>())
        {
            wnd::UpdateEvents(window.hWnd);
            dt::UpdateFPS();

            if (wnd::glo::window_h != 0 && wnd::glo::window_w != 0)
            {
                ptrGameScenes->Update(rpg::dt::seconds, *ptrHostResources);
                ptrRenderer->Render(rpg::dt::seconds, *ptrGameScenes, *ptrHostResources);
            }
        }

        gpu::vuk::VkCheck(vkDeviceWaitIdle(gpu::vuk::g_contextPtr->device));
        com::mem::PrintAllocationHTML();
    }
    
    com::mem::GlobalDeallocate();
    system("pause");

    return 0;
}


//TODO: rework vk image obj
//TODO: profiler
//TODO: normal per triangle on terrain instead of 
//TODO: IBO
//TODO: cast shadow or not (tree vs grass both texture)
//TODO: ECS serialization
//TODO: add static flag to render component, so ubo data is not updated
//TODO: join transform and render component
//TODO: draw stuff on render target for post fx
//TODO: use block ptr for s render function 