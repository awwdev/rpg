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

namespace mini::app
{
    struct Scene
    {
        //TODO: not every Scene will have those members
        ecs::ECS ecs {};
        rendering::RenderGraph renderGraph;
        rendering::Camera camera;
        res::Terrain terrain;

        Scene()
        {
            //TODO: move into some resource manager an load at loading scene
            //ecs.prefabs.Parse("res/prefabs.txt"); 

            {
                constexpr float S = 1; //sword is one 1x1 blender cube
                constexpr float X = 0; 
                constexpr float Y = 0;
                constexpr float Z = 0;
                const auto id = ecs.AddEntity();
                const math::Mat4f pos = math::Mat4f{
                    S, 0, 0, 0,
                    0, S, 0, 0,
                    0, 0, S, 0,
                    X, Y, Z, 1,
                };
                ecs.arrays.AddComponent<ecs::ComponentType::Transform> (id, pos);
                ecs.arrays.AddComponent<ecs::ComponentType::RenderData>(id, res::MeshType::Sword);
            }

            /*
            {
                constexpr float S =  0.5; //ths cube is -1 to 1 and half matches sword
                constexpr float X = -1; 
                constexpr float Y =  0;
                constexpr float Z =  0;
                const auto id = ecs.AddEntity();
                const math::Mat4f pos = math::Mat4f{
                    S, 0, 0, 0,
                    0, S, 0, 0,
                    0, 0, S, 0,
                    X, Y, Z, 1,
                };
                ecs.arrays.AddComponent<ecs::ComponentType::Transform> (id, pos);
                ecs.arrays.AddComponent<ecs::ComponentType::RenderData>(id, res::MeshType::PrimitiveCube);
            }
            */
            
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
            ui::DrawCamera(renderGraph, camera);

        }

    };

}//ns