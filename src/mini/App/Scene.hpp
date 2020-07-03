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
        ui::UI   ui  {};

        Scene()
        {
            //TODO: move into some resource manager an load at loading scene
            ecs.prefabs.Parse("res/prefabs.txt"); 

            ui.window1.rect  = { 128, 128, 300, 100 };
            ui.window1.title = "Window";
        }

        void Update(rendergraph::RenderGraph& renderGraph, const double dt)
        {
            //? UI
            ui::DrawFPS(renderGraph); 
            ui::DrawWindow(renderGraph, ui.window1);

            if (ui::DrawButton(renderGraph, "This is insane", {64, 64, 128, 24}))
            {
                LOG("pressed");
            }

        }

    };

}//ns