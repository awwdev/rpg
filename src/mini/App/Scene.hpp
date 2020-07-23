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

        ecs::ID cubeID = 0;


        Scene()
        {
            //TODO: move into some resource manager an load at loading scene
            //ecs.prefabs.Parse("res/prefabs.txt"); 

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

            
            {
                constexpr float S =  0.05f; //ths cube is -1 to 1 and half matches sword
                constexpr float X =  0; 
                constexpr float Y =  0;
                constexpr float Z =  0;
                cubeID = ecs.AddEntity();
                const math::Mat4f pos = math::Mat4f{
                    S, 0, 0, 0,
                    0, S, 0, 0,
                    0, 0, S, 0,
                    X, Y, Z, 1,
                };
                ecs.arrays.AddComponent<ecs::ComponentType::Transform> (cubeID, pos);
                ecs.arrays.AddComponent<ecs::ComponentType::RenderData>(cubeID, res::MeshType::PrimitiveCube);
            }
            
            
        }

        void Update(const double dt)
        {
            renderGraph.camera.Update(dt);

            //INTERSECTION TEST
            //TODO: now test against terrain 
            //TODO: move camera outside of rendergraph (same with terrain) later on ..
            
            using namespace utils;
            //const auto& trisTransform = ecs.arrays.transforms.Get(trisID).transform;
            //const auto v0  = trisTransform * MESH_TRIANGLE[0].pos;
            //const auto v1  = trisTransform * MESH_TRIANGLE[1].pos;
            //const auto v2  = trisTransform * MESH_TRIANGLE[2].pos;

            const auto ray = renderGraph.camera.ScreenRay();
            for(auto i = 0; i < renderGraph.terrain.CAPACITY; i+=3)
            {
                const auto& v0 = renderGraph.terrain[i+0].pos;
                const auto& v1 = renderGraph.terrain[i+1].pos;
                const auto& v2 = renderGraph.terrain[i+2].pos;
                const auto intersection = utils::RayTriangleIntersection(
                    renderGraph.camera.pos,
                    ray,
                    { v0[Vx], v0[Vy], v0[Vz] },
                    { v1[Vx], v1[Vy], v1[Vz] },
                    { v2[Vx], v2[Vy], v2[Vz] }
                );
                if (intersection) {
                    const auto X = intersection.t[Vx];
                    const auto Y = intersection.t[Vy];
                    const auto Z = intersection.t[Vz];

                    constexpr float S = 0.2f;
                    auto& cubeTrans = ecs.arrays.transforms.Get(cubeID);
                    cubeTrans.transform = {
                        S, 0, 0, 0,
                        0, S, 0, 0,
                        0, 0, S, 0,
                        X, Y, Z, 1,
                    };

                }
            }

            //TODO: vertex colors are cleared on resize, must be due to swapchain res recreation
            
            

            //? ECS
            ecs::S_Render(ecs.arrays, dt, renderGraph);

            //? UI
            ui::DrawFPS(renderGraph); 
            ui::DrawConsole(renderGraph);
            ui::DrawRenderStats(renderGraph);
            ui::DrawCameraPos(renderGraph, renderGraph.camera);

        }

    };

}//ns