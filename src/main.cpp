//https://github.com/awwdev

#include "mini/Window/win_Window.hpp"
#include "mini/Window/win_WindowCallback.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Debug/Console.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Box/Map.hpp"
#include "mini/Box/StringMap.hpp"
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
        auto ptrGameScenes = mem::ClaimBlock<box::Array<app::GameScene, 1, box::INIT::Yes>>();
        uint32_t gameSceneIdx = 0;

        while (!wnd::CheckEvent(EventType::Window_Close) && !wnd::IsPressed(EventType::Keyboard_Escape))
        {
            wnd::PollEvents(window.hWnd);
            dt::UpdateFPS();

            if (wnd::window_h != 0 && wnd::window_w != 0)
            {
                ptrGameScenes[gameSceneIdx].renderGraph.Clear();
                ptrGameScenes[gameSceneIdx].Update(mini::dt::seconds);
                ptrRenderer->Render(mini::dt::seconds, ptrGameScenes[gameSceneIdx]);
            }
        }

        VK_CHECK(vkDeviceWaitIdle(vk::g_contextPtr->device));
    }
    
    mem::GlobalDeallocate();
    system("pause");

} //main end


//TODO: plane grid, terrain editing