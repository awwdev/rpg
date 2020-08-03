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
#include "mini/Resources/Terrain/Terrain.hpp"
#include "mini/Rendering/Sun.hpp"

#include "mini/App/PlayerController.hpp"
#include "mini/App/Input.hpp"

namespace mini::app
{
    struct GameScene
    {
        //TODO: not every Scene will have those members
        ecs::ECS                ecs {};
        rendering::RenderGraph  renderGraph;
        rendering::Camera       camera;
        rendering::Sun          sun;

        app::PlayerController playerController;
        

        GameScene()
        {
            //TODO: move into some resource manager an load at loading scene
            //ecs.prefabs.Parse("res/prefabs.txt"); 
            sun.Create(ecs);
            playerController.Create(ecs);
        }


        void Update(const double dt, res::HostResources& hostRes)
        {
            renderGraph.Clear();
            app::UpdateInputMode();
            camera.Update(dt);
            
            //? META
            hostRes.terrain.Update(dt, camera, ecs);
            sun.Update(ecs, dt);
            playerController.Update(ecs, dt, camera);

            //? ECS
            ecs::S_Render(ecs.arrays, dt, renderGraph);

            //? UI
            ui::DrawFPS(renderGraph); 
            if (app::global::UI_Mode)  {
                ui::DrawConsole(renderGraph);
                ui::DrawRenderStats(renderGraph);
                ui::DrawCameraPos(renderGraph, camera);
            }
           
        }

    };

}//ns