//https://github.com/awwdev

#pragma once

#include "ecs/ECS.hpp"

#include "gpu/RenderData/RenderData.hpp"
#include "gpu/Meta/Sun.hpp"

#include "gui/Editor/GUI_Stats.hpp"
#include "gui/Editor/GUI_Shadow.hpp"
#include "gui/Editor/GUI_Level.hpp"
#include "gui/Editor/GUI_Scene.hpp"

#include "res/Terrain/_Old/Terrain.hpp"
#include "res/Prefab/PrefabLoader.hpp"
#include "res/Resources.hpp"

#include "app/Player/PlayerController.hpp"
#include "app/Editor/EditorController.hpp"
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
    gui::GUI_Scene  guiScene;

    GameScene(res::Resources& resources)
    {
        guiLevel.Init(resources.prefabs);
        sun.Create(ecs);
        playerController.Create(ecs);
        ecs.prefabsArrays = resources.prefabs.prefabsArrays;    
    }

    void Update(const double dt, res::Resources& resources)
    {
        renderData.Clear();
        renderData.general.meta.time += dt;     

        //? ui
        app::ResetUpdateInputMode();
        if (app::glo::inputMode != app::glo::InputMode::PlayMode) {
            guiLevel.Update(renderData, resources, editorController.editorMode);
            guiStats.Update(renderData);
            guiScene.Update(renderData, ecs);
        }   

        //? meta
        if (app::glo::inputMode == app::glo::InputMode::PlayMode) 
            playerController.Update(dt, ecs, renderData);
        else 
            editorController.Update(dt, ecs, resources, renderData);

        sun.Update(ecs, dt, renderData);

        //? ecs
        ecs.Update(dt, renderData);
    }

};

}//nsooo