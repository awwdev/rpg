//https://github.com/awwdev

#pragma once

#include "mini/ECS/ECS.hpp"
#include "mini/ECS/Prefabs.hpp"
#include "mini/Utils/Types.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Box/String.hpp"
#include "mini/Rendering/RenderGraph.hpp"
#include "mini/App/UI.hpp"

namespace mini::app
{
    struct Scene
    {
        ecs::ECS ecs {};
        rendering::RenderGraph renderGraph;

        Scene()
        {
            //TODO: move into some resource manager an load at loading scene
            ecs.prefabs.Parse("res/prefabs.txt"); 
        }

        void Update(const double dt)
        {
            //TEST: update ubo data (transform)
            //TODO: ECS provides render data ("render system")
            static float rot = 0;
            rot += 1.f * (float)dt;
            auto rotMat1 = math::RotationMatrixX(0.5);
            auto rotMat2 = math::RotationMatrixY(rot);
            rotMat2 *= rotMat1;
            rotMat2[3][2] = -5;
            rotMat2[3][0] = 0;
            rendering::UniformData_Default cubes [] = {
                rotMat2
            };
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