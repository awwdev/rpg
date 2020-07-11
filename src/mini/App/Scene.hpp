//https://github.com/awwdev

#pragma once

#include "mini/ECS/ECS.hpp"
#include "mini/ECS/Prefabs.hpp"
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
            ecs.prefabs.Parse("res/prefabs.txt"); 
            camera.x = -100;
            camera.y = - 50;
        }

        void Update(const double dt)
        {
            camera.Update(dt);

            //ecs, render component mesh id


            //TEST: update ubo data (transform)
            //TODO: ECS provides render data ("render system")
            static float rot = 0;
            rot += 1.f * (float)dt;

            rendering::UniformData_Default cubes [500];
            for(auto y = 0; y < 10; ++y)
            for(auto x = 0; x < 50; ++x)
            {
                auto rotMat1 = math::RotationMatrixX(0.5f+y);
                auto rotMat2 = math::RotationMatrixY(rot);
                rotMat2 *= rotMat1;
                rotMat2[3][2] = -100.f;
                rotMat2[3][0] = x * 5.f;
                rotMat2[3][1] = y * 5.f;
                cubes [y * 50 + x] = {
                    rotMat2
                };
            }
            
            //! ORDERING IS CURRENTLY IMPORTANT
            //TODO: SORTING; MESH ID -> ARRAY ID -> UBO VERTICAL ALIGN
            renderGraph.default_uboGroups.Clear();
            renderGraph.default_uboArray.Clear();

            renderGraph.default_uboGroups.Append(rendering::UniformGroup{ 
                .begin = renderGraph.default_uboArray.count, 
                .count = (u32)ARRAY_COUNT(cubes) 
            });
            renderGraph.default_uboArray.AppendArray(cubes);

            
            //? UI
            ui::DrawFPS(renderGraph); 
            ui::DrawConsole(renderGraph);

        }

    };

}//ns