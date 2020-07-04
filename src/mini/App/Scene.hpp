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
            ui.input1.input  = "123";
        }

        void Update(rendergraph::RenderGraph& renderGraph, const double dt)
        {
            //? UI
            ui::DrawFPS(renderGraph); 

            ui::DrawWindow(renderGraph, ui.window1);
            if (ui::DrawButton(renderGraph, "Button", {8, 8, 128, 24}, ui.window1)) {
                LOG("pressed button");
            }
            if (ui::DrawInputField(renderGraph, ui.input1, "Input", {8, 40, 300 - 16, 24}, ui.window1)) {
                LOG("pressed input field");
            }

        }

    };

}//ns