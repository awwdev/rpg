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
#include "mini/Rendering/UI.hpp"
#include "mini/Resources/Terrain.hpp"
#include "mini/Rendering/Sun.hpp"

namespace mini::app
{
    struct GameScene
    {
        //TODO: not every Scene will have those members
        ecs::ECS                ecs {};
        rendering::RenderGraph  renderGraph;
        res::Terrain            terrain;
        rendering::Camera       camera;
        rendering::Sun          sun;

        GameScene()
        {
            //TODO: move into some resource manager an load at loading scene
            //ecs.prefabs.Parse("res/prefabs.txt"); 
            terrain.Create(ecs);

            /*{
                constexpr float S = 1; //sword is one 1x1 blender cube
                constexpr float X = 0; 
                constexpr float Y = 0;
                constexpr float Z =-4;
                trisID = ecs.AddEntity();
                const math::Mat4f pos = math::Mat4f{
                    S, 0, 0, 0,
                    0, S, 0, 0,
                    0, 0, S, 0,
                    X, Y, Z, 1,
                };
                ecs.arrays.AddComponent<ecs::ComponentType::Transform> (trisID, pos);
                ecs.arrays.AddComponent<ecs::ComponentType::RenderData>(trisID, res::MeshType::PrimitiveTriangle);
            }*/

        }


        void Update(const double dt)
        {
            //? META
            ui::Update();
            camera.Update(dt);
            terrain.Update(dt, camera, ecs);
            sun.Update(dt);

            //? ECS
            ecs::S_Render(ecs.arrays, dt, renderGraph);

            //? UI
            ui::DrawFPS(renderGraph); 
            ui::DrawConsole(renderGraph);
            ui::DrawRenderStats(renderGraph);
            ui::DrawCameraPos(renderGraph, camera);
        }

    };

}//ns