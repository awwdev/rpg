//https://github.com/awwdev

#pragma once

#include "ECS/ECS.hpp"
#include "ECS/Systems/S_Render.hpp"

#include "GPU/RenderData/RenderData.hpp"
#include "GPU/Meta/Sun.hpp"

#include "GUI/Editor/GUI_Stats.hpp"
#include "GUI/Editor/GUI_Shadow.hpp"
#include "GUI/Editor/GUI_Level.hpp"

#include "Resources/Terrain/Terrain.hpp"
#include "ECS/Prefabs/PrefabLoader.hpp"

#include "App/PlayerController.hpp"
#include "App/EditorController.hpp"
#include "App/InputMode.hpp"

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

    void Create(res::CpuResources& hostRes)
    {
        //TODO: resource manager, init gizmos? find better place!
        res::LoadPrefabs("AssetsApp/prefabs.mini", ecs.prefabsArrays);
        sun.Create(ecs);
        playerController.Create(ecs);
        hostRes.terrain.InitGizmos(ecs);
    }


    void Update(const double dt, res::CpuResources& hostRes)
    {
        renderData.Clear();

        //? UI
        app::ResetUpdateInputMode();
        if (app::glo::inputMode != app::glo::InputMode::PlayMode) {
            guiLevel.Update(renderData);
            guiStats.Update(renderData);
            //guiShadow.Update(renderData);
        }   

        //? META
        playerController.Update(dt, ecs, renderData);
        editorController.Update(dt, ecs, renderData);
        hostRes.terrain.Update(dt, editorController.camera, ecs); //move into editor?
        sun.Update(ecs, dt, renderData);

        //? ECS
        ecs::S_Render(ecs.arrays, dt, renderData);
    }

};

}//nsooo