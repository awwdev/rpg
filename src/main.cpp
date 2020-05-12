#include "mini/Window/win_Window.hpp"
#include "mini/Window/win_WindowCallback.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Debug/Console.hpp"
#include "mini/Debug/Profiler.hpp"
#include "mini/Box/String.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Memory/Allocator.hpp"
#include "rpgcpp/Scene/Scene.hpp"

using namespace mini;

int WINAPI wWinMain(
    _In_        HINSTANCE hInstance,
    _In_opt_    HINSTANCE hPrevInstance,
    _In_        PWSTR pCmdLine,
    _In_        int nCmdShow)
{
    //meta
    const auto con = dbg::CreateConsole();
    const auto wnd = wnd::mini_CreateWindow(hInstance, 800, 600);
    mem::Allocate();
    
    //scenes
    using SceneStack = mini::box::Array<rpgcpp::scene::Scene, 4>;
    auto sceneStack  = mini::mem::ClaimBlock<SceneStack>();
    sceneStack->SetCompleteArray();
    auto& currentScene = (*sceneStack)[0];

    while (!app::CheckEvent(EventType::Window_Close) && !app::IsPressed(EventType::Keyboard_Escape))
    {
        wnd::PollEvents();
        
        currentScene.Update();
        currentScene.Update();
    }

}//main end

//todo: DELTA TIME