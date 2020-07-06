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
        wnd::Window window{hInstance, 800, 600};
        mem::GlobalAllocate();
        hostRes::HostResources hostResources{}; //will load immediately for now

        constexpr auto s = sizeof vk::VkRenderer;
        auto ptrRenderer = mem::ClaimBlock<vk::VkRenderer>(
            vk::WindowHandle{window.hInstance, window.hWnd},
            hostResources);

        auto ptrSceneStack = mem::ClaimBlock<box::Array<app::Scene, 1, box::INIT::Yes>>();
        uint32_t sceneIdx = 0;

        while (!wnd::CheckEvent(EventType::Window_Close) && !wnd::IsClicked(EventType::Keyboard_Escape))
        {
            wnd::PollEvents();
            dt::UpdateFPS();

            if (wnd::window_h != 0 && wnd::window_w != 0)
            {
                rendergraph::renderGraph.Clear();
                ptrSceneStack[sceneIdx].Update(mini::dt::seconds);
                ptrRenderer->Render(mini::dt::seconds, ptrSceneStack[sceneIdx]);
            }
        }

        VK_CHECK(vkDeviceWaitIdle(vk::context.device));
    }

    mem::GlobalDeallocate();
    system("pause");

} //main end

//TODO: Resource Manager