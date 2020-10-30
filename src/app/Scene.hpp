//https://github.com/awwdev

#pragma once

#include "ecs/ECS.hpp"

#include "gpu/RenderData/RenderData.hpp"
#include "gpu/Meta/Sun.hpp"

#include "gui/Editor/GUI_Stats.hpp"
#include "gui/Editor/GUI_Shadow.hpp"
#include "gui/Editor/GUI_Level.hpp"
#include "gui/Editor/GUI_Scene.hpp"

#include "res/Prefab/PrefabLoader.hpp"
#include "res/Resources.hpp"

#include "app/Player/Player.hpp"
#include "app/Editor/Editor.hpp"
#include "app/InputMode.hpp"

namespace rpg::app {
    
struct GameScene
{
    ecs::ECS ecs;
    gpu::RenderData renderData;
    gpu::Sun sun;

    app::Player player;
    app::Editor editor;

    gui::GUI_Level  guiLevel;
    gui::GUI_Shadow guiShadow;
    gui::GUI_Stats  guiStats;
    gui::GUI_Scene  guiScene;

    GameScene(res::Resources& resources)
    {
        guiLevel.Init(resources.prefabs);
        sun.Create();
        player.Create(ecs);
        ecs.prefabsArrays = resources.prefabs.prefabsArrays;    
        editor.CreateGizmos(ecs);

        auto testID = ecs.AddEntity(res::PrefabEnum::Cube);
        auto& mainComponent = ecs.arrays.mainComponents.Get(testID);
        mainComponent.translation.z = -5;
    }

    void Update(const double dt, res::Resources& resources)
    {
        renderData.Clear();
        renderData.general.meta.time += dt;     

        //? ui
        app::ResetUpdateInputMode();
        if (app::glo::inputMode != app::glo::InputMode::PlayMode) {
            guiLevel.Update(renderData, resources, editor);
            guiStats.Update(renderData);
            guiScene.Update(renderData, ecs);
        }   

        //? meta
        if (app::glo::inputMode == app::glo::InputMode::PlayMode) 
            player.Update(dt, ecs, renderData);
        else 
            editor.Update(dt, ecs, resources, renderData);

        sun.Update(editor.camera); //TODO: active camera ptr instead of editor camera
        sun.UpdateRenderData(renderData);

        //? ecs
        ecs.Update(dt, renderData);

    }

};

}//ns