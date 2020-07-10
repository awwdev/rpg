//https://github.com/awwdev

#pragma once

#include "mini/ECS/ECS.hpp"
#include "mini/ECS/Prefabs.hpp"
#include "mini/Utils/Types.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Box/String.hpp"
#include "mini/RenderGraph/RenderGraph.hpp"
#include "mini/App/UI.hpp"

namespace mini::app
{
    struct Scene
    {
        ecs::ECS ecs {};
        rendergraph::RenderGraph renderGraph;

        Scene()
        {
            //TODO: move into some resource manager an load at loading scene
            ecs.prefabs.Parse("res/prefabs.txt"); 
            rendergraph::g_renderGraphPtr = &renderGraph;
        }

        void Update(const double dt)
        {
            //TEST: update ubo data (transform)
            //TODO: ECS provides render data
            rendergraph::UniformData_Default triangles [] = {
                math::Identity4x4(),
                math::Identity4x4(),
            };
            rendergraph::UniformData_Default quads [] = {
                { 
                    .1f, 0.f, 0.f, .5f,
                    0.f, .1f, 0.f, 0.f,
                    0.f, 0.f, .1f, 0.f,
                    0.f, 0.f, 0.f, 1.f,            
                },
                { 
                    .1f, 0.f, 0.f, -.5f,
                    0.f, .1f, 0.f, 0.f,
                    0.f, 0.f, .1f, 0.f,
                    0.f, 0.f, 0.f, 1.f,            
                },
            };
            //! ORDERING IS CURRENTLY IMPORTANT
            //TODO: SORTING; MESH ID -> ARRAY ID -> UBO VERTICAL ALIGN

            renderGraph.default_uboGroups.Clear();
            renderGraph.default_uboArray.Clear();
            
            renderGraph.default_uboGroups.Append(rendergraph::UniformGroup{ 
                .begin = renderGraph.default_uboArray.count, 
                .count = (u32)ARRAY_COUNT(triangles) 
            });
            renderGraph.default_uboArray.AppendArray(triangles);

            renderGraph.default_uboGroups.Append(rendergraph::UniformGroup{ 
                .begin = renderGraph.default_uboArray.count, 
                .count = (u32)ARRAY_COUNT(quads) 
            });
            renderGraph.default_uboArray.AppendArray(quads);
            
            
            

            

            ui::DrawFPS(); 
            ui::DrawConsole();

        }

    };

}//ns