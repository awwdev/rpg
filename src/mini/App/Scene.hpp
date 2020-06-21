#pragma once
#include "mini/ECS/ECS.hpp"
#include "mini/ECS/Prefabs.hpp"
#include "mini/Utils/Types.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Box/String.hpp"
#include "mini/App/IRenderer.hpp"
#include "mini/App/UI.hpp"

using namespace mini;

namespace mini::app
{
    struct Scene
    {
        box::String<10> name { "Scene1" };
        ecs::ECS ecs;

        ecs::ID FpsMonitor;
        ecs::ID Button1;

        Scene()
        {
            ecs.prefabs.Parse("res/prefabs.txt"); //TODO: move somewhere else?
            FpsMonitor = ecs.AddEntity(ecs::PrefabType::UI_FpsMonitor);
            Button1    = ecs.AddEntity(ecs::PrefabType::UI_Button);
        }

        inline void Update(IRenderer& renderer, const double dt)
        {
            ui::ProcessComponents_UI(renderer, ecs.arrays.uiData.dense);

            //custom logic
            ui::UpdateFpsMonitorText(renderer, dt, ecs.arrays.uiData.Get(FpsMonitor));
            if (ecs.arrays.uiData.Get(Button1).state == ecs::C_UI::Released)
                LOG("button released");
            
        }

    };

}//ns