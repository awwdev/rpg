//https://github.com/awwdev

#include "mini/Window/win_Window.hpp"
#include "mini/Window/win_WindowCallback.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Debug/Console.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Utils/Types.hpp"
#include "mini/Memory/Allocator.hpp"
#include "mini/Memory/AllocatorPrint.hpp"
#include "mini/Vulkan/Rendering/Renderer.hpp"

#include "mini/App/Scene.hpp"
#include "mini/Utils/DeltaTime.hpp"
#include "mini/Resources/HostResources.hpp"

#include "mini/Box/String.hpp"

using namespace mini;
using namespace mini::wnd;

int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ PWSTR pCmdLine,
    _In_ int nCmdShow)
{
    {
        dbg::Console console{};
        wnd::Window  window { hInstance, 800, 600 };
        mem::GlobalAllocate();
        auto ptrHostResources = mem::ClaimBlock<res::HostResources>(); //will load immediately for now

        auto ptrRenderer = mem::ClaimBlock<vk::VkRenderer>(
            vk::WindowHandle{window.hInstance, window.hWnd},
            *ptrHostResources);

        //TODO: make stack for each scene type instead of one generic scene
        auto ptrGameScenes = mem::ClaimBlock<app::GameScene>();
        ptrGameScenes->Create(*ptrHostResources);

        while (wnd::global::events[wnd::Window_Close] == wnd::None && wnd::global::events[wnd::ESC] == wnd::None)
        {
            wnd::UpdateEvents(window.hWnd);
            dt::UpdateFPS();

            if (wnd::global::window_h != 0 && wnd::global::window_w != 0)
            {
                ptrGameScenes->Update(mini::dt::seconds, *ptrHostResources);
                ptrRenderer->Render(mini::dt::seconds, *ptrGameScenes, *ptrHostResources);
            }
        }

        VK_CHECK(vkDeviceWaitIdle(vk::g_contextPtr->device));
        mem::PrintAllocationHTML();
    }
    
    mem::GlobalDeallocate();
    system("pause");

} //main end

//TODO: draw stuff on render target for post fx
//TODO: terrain refactor