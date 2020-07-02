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
        ecs::ECS ecs;

        Scene()
        {
            //TODO: move into some resource manager an load at loading scene
            ecs.prefabs.Parse("res/prefabs.txt"); 
        }

        void Update(rendergraph::RenderGraph& renderGraph, const double dt)
        {
            //add immediate ui
            if (ui::DrawButton(renderGraph, "Load", {64, 64, 256, 32}))
            {
                LOG("pressed");
            }

            if (ui::DrawButton(renderGraph, "Options", {64, 128, 256, 32}))
            {
                LOG("pressed");
            }
        }

    };

}//ns