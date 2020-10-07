//https://github.com/awwdev

#pragma once

#include "ecs/ECS.hpp"

#include "gpu/RenderData/RenderData.hpp"
#include "gpu/Meta/Sun.hpp"

#include "gui/Editor/GUI_Stats.hpp"
#include "gui/Editor/GUI_Shadow.hpp"
#include "gui/Editor/GUI_Level.hpp"
#include "gui/Editor/GUI_Scene.hpp"

#include "res/Terrain/Terrain.hpp"
#include "res/Prefab/PrefabLoader.hpp"
#include "res/Resources.hpp"

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
    gui::GUI_Scene  guiScene;

    ecs::ID swordID = 0; //TEST

    GameScene(res::Resources& resources)
    {
        guiLevel.Init(resources.prefabs);
        
        sun.Create(ecs);
        playerController.Create(ecs);
        resources.terrain.terrain.InitGizmos(ecs);
        ecs.prefabsArrays = resources.prefabs.prefabsArrays;

        //TEST
        {
            //swordID = ecs.AddEntity(res::PrefabEnum::Sword);
            //ecs::MainComponent& swordMainComponent = ecs.arrays.mainComponents.Get(swordID);
            //swordMainComponent.translation = { 2, 0, 2 };
        }       
    }

    void Update(const double dt, res::Resources& resources)
    {
        //TEST
        //auto& swordMainComponent = ecs.arrays.mainComponents.Get(swordID);
        //swordMainComponent.rotation.y += dt * 100;

        renderData.Clear();
        renderData.general.meta.time += dt;     

        //? UI
        app::ResetUpdateInputMode();
        if (app::glo::inputMode != app::glo::InputMode::PlayMode) {
            guiLevel.Update(renderData, resources);
            guiStats.Update(renderData);
            guiScene.Update(renderData);
        }   

        //? META
        if (app::glo::inputMode == app::glo::InputMode::PlayMode) 
            playerController.Update(dt, ecs, renderData);
        else 
            editorController.Update(dt, ecs, renderData);

        resources.terrain.terrain.Update(dt, editorController.camera, ecs); //move into editor?
        sun.Update(ecs, dt, renderData);

        //? ECS
        ecs.Update(dt, renderData);
    }

};

}//nsooo