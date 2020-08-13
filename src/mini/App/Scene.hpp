//https://github.com/awwdev

#pragma once

#include "mini/ECS/ECS.hpp"
#include "mini/ECS/Prefabs.hpp"
#include "mini/ECS/Systems/S_Render.hpp"

#include "mini/Utils/Types.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Box/String.hpp"
#include "mini/Rendering/RenderGraph.hpp"
#include "mini/Rendering/UI.hpp"
#include "mini/Resources/Terrain/Terrain.hpp"
#include "mini/Rendering/Sun.hpp"

#include "mini/App/PlayerController.hpp"
#include "mini/App/EditorController.hpp"
#include "mini/App/InputMode.hpp"

namespace mini::app {
    
struct GameScene
{
    //TODO: not every Scene will have those members
    ecs::ECS                ecs {};
    rendering::RenderGraph  renderGraph;
    rendering::Sun          sun;

    app::PlayerController playerController;
    app::EditorController editorController;
    
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
        playerController.Update(dt, ecs);
        editorController.Update(dt, ecs);
        hostRes.terrain.Update(dt, editorController.camera, ecs); //move into editor?
        sun.Update(ecs, dt);

        //? ECS
        ecs::S_Render(ecs.arrays, dt, renderGraph);

        //? UI
        ui::DrawFPS(renderGraph); 
        if (app::global::UI_Mode)  {
            ui::DrawConsole(renderGraph);
            ui::DrawRenderStats(renderGraph);
            ui::DrawCameraPos(renderGraph, editorController.camera);
            ui::DrawTerrainData(renderGraph, hostRes.terrain);
        }
        
    }

};

}//ns