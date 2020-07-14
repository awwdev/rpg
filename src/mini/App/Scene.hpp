//https://github.com/awwdev

#pragma once

#include "mini/ECS/ECS.hpp"
#include "mini/ECS/Prefabs.hpp"
#include "mini/ECS/Systems/S_Render.hpp"

#include "mini/Utils/Types.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Box/String.hpp"
#include "mini/Rendering/RenderGraph.hpp"
#include "mini/Rendering/Camera.hpp"
#include "mini/App/UI.hpp"

namespace mini::app
{
    struct Scene
    {
        ecs::ECS ecs {};
        rendering::RenderGraph renderGraph;
        rendering::Camera camera;

        Scene()
        {
            //TODO: move into some resource manager an load at loading scene
            //ecs.prefabs.Parse("res/prefabs.txt"); 

            const auto cubeID1 = ecs.AddEntity();
            const auto rot = math::RotationMatrixX(-1.5708);
            const math::Mat4f pos = math::Mat4f{
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0,-.2f,-2, 1,
            };
            ecs.arrays.AddComponent<ecs::ComponentType::Transform>(cubeID1, rot * pos);
            ecs.arrays.AddComponent<ecs::ComponentType::RenderData>(cubeID1, resources::MeshType::Sword);
        }

        void Update(const double dt)
        {
            camera.Update(dt);

            //? ECS
            ecs::S_Render(ecs.arrays, dt, renderGraph);

            //? UI
            ui::DrawFPS(renderGraph); 
            ui::DrawConsole(renderGraph);
            ui::DrawRenderStats(renderGraph);
        }

    };

}//ns