//https://github.com/awwdev

#pragma once

#include "ECS/ECS.hpp"
#include "ECS/Systems/S_Render.hpp"

#include "Rendering/RenderGraph.hpp"
#include "Rendering/Sun.hpp"

#include "UI/UI_Shadow.hpp"
#include "UI/UI_Terrain.hpp"
#include "UI/UI_Stats.hpp"

#include "UI/Widgets/Widget_Window.hpp"
#include "UI/Widgets/Widget_List.hpp"

#include "Resources/Terrain/Terrain.hpp"
#include "ECS/Prefabs/PrefabLoader.hpp"

#include "App/PlayerController.hpp"
#include "App/EditorController.hpp"
#include "App/InputMode.hpp"

namespace rpg::app {
    
struct GameScene
{
    //TODO: not every Scene will have those members
    ecs::ECS                ecs {};
    gpu::RenderData  renderData;
    gpu::Sun          sun;

    app::PlayerController playerController;
    app::EditorController editorController;

    void Create(res::HostResources& hostRes)
    {
        //TODO: resource manager, init gizmos? find better place!
        res::LoadPrefabs("res/prefabs.mini", ecs.prefabsArrays);
        sun.Create(ecs);
        playerController.Create(ecs);
        hostRes.terrain.InitGizmos(ecs);
        ui::g_aciveRenderGraph = &renderData;
    }


    void Update(const double dt, res::HostResources& hostRes)
    {
        renderData.Clear();

        //? UI
        app::ResetUpdateInputMode();
        if (app::global::inputMode != app::global::InputMode::PlayMode) {
            ui::DrawUI_Terrain(hostRes.terrain);
            ui::DrawUI_Stats();
            ui::DrawUI_Shadow(sun);

            static dbg::gui::Widget_Window wnd { .title = "Test", .rect { 100, 100, 300, 300 }};
            static dbg::gui::Widget_List<100> list { .name = "List", .rect { 100, 100, 300, 300 } };
            list.items.Clear();

            for(auto i = 0; i < 2; ++i) {
                decltype(list.items)::TYPE str { "Item " };
                str.Append(i);
                list.items.Append(str);
            }

            wnd.Update(renderData);
            list.Update(renderData, wnd);
        }   

        //? META
        playerController.Update(dt, ecs);
        editorController.Update(dt, ecs);
        hostRes.terrain.Update(dt, editorController.camera, ecs); //move into editor?
        sun.Update(ecs, dt);

        //? ECS
        ecs::S_Render(ecs.arrays, dt, renderData);
    }

};

}//ns