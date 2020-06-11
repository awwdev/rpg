#pragma once
#include "mini/App/ECS.hpp"
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
            
        }

        inline void Update(IRenderer& renderer, const double dt)
        {
            ui::FPS_Monitor(renderer, dt);

            if (ui::Button(renderer, "Add Entity", {100, 200, 120, 30}) == ui::ButtonState::Released) {
                const auto id = ecs.AddEntity();
                ecs.AddComponent<ecs::C_Transform>(id, math::Vec3f{1, 2, 3});
            }

            if (ui::Button(renderer, "Rmv Entity", {100, 232, 120, 30}) == ui::ButtonState::Released) {
                ecs.RemoveComponent<ecs::C_Transform>(0);
                //ecs.RemoveEntity(0);
            }

            //? PRINT ECS STUFF
            for(auto i = 0; i < ecs.entityCount; ++i)
            {
                char buf[2];

                //? entity id
                sprintf_s(buf, "%d", i);
                renderer.Add_DrawLabel(buf, { 100 + (int)i*33, 33*0, 32, 32});

                //? lookup
                sprintf_s(buf, "%d", ecs.arrays.transforms.lookup[i]);
                renderer.Add_DrawLabel(buf, { 100 + (int)i*33, 33*1, 32, 32});

                //? dense
                if (i < ecs.arrays.transforms.dense.Count()) {
                    sprintf_s(buf, "%d", i);
                    renderer.Add_DrawLabel(buf, { 100 + (int)i*33, 33*2, 32, 32});
                }

                //? reverse
                sprintf_s(buf, "%d", ecs.arrays.transforms.reverse[i]);
                renderer.Add_DrawLabel(buf, { 100 + (int)i*33, 33*3, 32, 32});
            } 



            
            
        }

    };

}//ns