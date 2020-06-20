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

        Scene()
        {
            ecs.prefabs.Parse("res/prefabs.txt"); //TODO: move somewhere else?
            ecs.AddEntity(ecs::PrefabType::UI_FpsMonitor);
            ecs.AddEntity(ecs::PrefabType::UI_FpsMonitor);
            //TODO: add buttons (button logic where?)
        }

        inline void Update(IRenderer& renderer, const double dt)
        {
            //TODO: just call system functions: draw ui entities
            

            ui::FPS_Monitor(renderer, dt);

            if (ui::Button(renderer, "Add Entity", {100, 200, 120, 30}) == ui::ButtonState::Released) {
                //const auto id = ecs.AddEntity();
                //ecs.AddComponent<ecs::C_Transform>(id, math::Vec3f{1, 2, 3});
            }

            if (ui::Button(renderer, "Rmv Entity", {100, 232, 120, 30}) == ui::ButtonState::Released) {
                ////ecs.RemoveComponent<ecs::C_Transform>(0);
                //ecs.RemoveEntity(0);
            }

            //? PRINT ECS STUFF
            for(auto i = 0; i < 9; ++i)
            {
                char buf[10] = "e";

                //? entity id
                if (ecs.entities.Test(i))
                    sprintf_s(buf, "%d", i);
                renderer.Add_DrawLabel(buf, { 100 + (int)i*33, 33*0, 32, 32});

                //? lookup
                sprintf_s(buf, "%d", ecs.arrays.uiData.componentLookup[i]);
                renderer.Add_DrawLabel(buf, { 100 + (int)i*33, 33*1, 32, 32});

                //? dense
                if (i < ecs.arrays.uiData.dense.Count()) {
                    sprintf_s(buf, "%d", i);
                    renderer.Add_DrawLabel(buf, { 100 + (int)i*33, 33*2, 32, 32});
                }

                //? reverse
                sprintf_s(buf, "%d", ecs.arrays.uiData.entityLookup[i]);
                renderer.Add_DrawLabel(buf, { 100 + (int)i*33, 33*3, 32, 32});
            } 



            
            
        }

    };

}//ns