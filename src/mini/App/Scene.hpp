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

            //? UI 
            ui.console.rect   = { 8, wnd::window_h - 8 - 100.f, wnd::window_w * 0.75f, 100 };
            ui.console.limits = { 100, 100, 800, 300 };
            ui.console.title  = "Console";

            ui.consoleInput.input  = "123";
        }

        void Update(rendergraph::RenderGraph& renderGraph, const double dt)
        {
            ui::DrawFPS(renderGraph); 
            ui::DrawConsole(renderGraph, ui);

        }

    };

}//ns