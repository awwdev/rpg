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

#include "mini/Scene/Scene.hpp"
#include "mini/Utils/DeltaTime.hpp"
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
        res::ResourceManager resourceManager {}; //will load immediately for now

        //? RENDERER
        auto ptrRenderer = mem::ClaimBlock<vk::Renderer>(
            vk::WindowHandle{window.hInstance, window.hWnd},
            resourceManager
        );
        
        //? SCENES
        auto ptrSceneStack = mem::ClaimBlock<box::Array<app::scene::Scene, 1>>();
        ptrSceneStack->InitCompleteArray();
        uint32_t sceneIdx = 0;

        //? PROGRAM LOOP
        while (!wnd::CheckEvent(EventType::Window_Close) && !wnd::IsPressed(EventType::Keyboard_Escape))
        {
            wnd::PollEvents();
            app::dt::CalcDeltaTimeFPS();        
            ptrSceneStack[sceneIdx].Update(app::dt::seconds);
            ptrRenderer->Render(app::dt::seconds, ptrSceneStack[sceneIdx]);
        }
        
        //? THE END
        VK_CHECK(vkDeviceWaitIdle(ptrRenderer->context.device));
    }

    mem::GlobalDeallocate();
    system("pause");

}//main end


//write better: texture array stuff inside res mang
//make more obvious texture image connection

//join shader default and pipeline r+ renderpass under folder dedicated
//text renderer 
//vb buffer, texture atlas texture coords