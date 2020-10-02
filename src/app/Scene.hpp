//https://github.com/awwdev

#pragma once

#include "ecs/ECS.hpp"
#include "ecs/Systems/RenderSystem.hpp"

#include "gpu/RenderData/RenderData.hpp"
#include "gpu/Meta/Sun.hpp"

#include "gui/Editor/GUI_Stats.hpp"
#include "gui/Editor/GUI_Shadow.hpp"
#include "gui/Editor/GUI_Level.hpp"

#include "res/Terrain/Terrain.hpp"
#include "res/Prefabs/PrefabLoader.hpp"

#include "app/PlayerController.hpp"
#include "app/EditorController.hpp"
#include "app/InputMode.hpp"

namespace rpg::app {
    
struct GameScene
{
    //TODO: not every Scene will have those members
    ecs::ECS          ecs;
    gpu::RenderData   renderData;
    gpu::Sun          sun;

    app::PlayerController playerController;
    app::EditorController editorController;

    gui::GUI_Level  guiLevel;
    gui::GUI_Shadow guiShadow;
    gui::GUI_Stats  guiStats;

    void Create(res::CpuResources& cpuRes)
    {
        sun.Create(ecs);
        playerController.Create(ecs);
        cpuRes.terrain.InitGizmos(ecs);
    }


    void Update(const double dt, res::CpuResources& cpuRes)
    {
        renderData.Clear();

        //? UI
        app::ResetUpdateInputMode();
        if (app::glo::inputMode != app::glo::InputMode::PlayMode) {
            guiLevel.Update(renderData, cpuRes);
            guiStats.Update(renderData);
            //guiShadow.Update(renderData);
        }   

        //? META
        if (app::glo::inputMode == app::glo::InputMode::PlayMode) 
            playerController.Update(dt, ecs, renderData);
        else 
            editorController.Update(dt, ecs, renderData);

        cpuRes.terrain.Update(dt, editorController.camera, ecs); //move into editor?
        sun.Update(ecs, dt, renderData);

        //? ECS
        ecs::RenderSystem(ecs.arrays, dt, renderData);
    }

};

}//nsooo