//https://github.com/awwdev

#pragma once
#include "ecs/ECS.hpp"
#include "com/Matrix.hpp"
#include "wnd/WindowEvents.hpp"
#include "gpu/Meta/Cameras.hpp"
#include "app/InputMode.hpp"
#include "gpu/RenderData/RenderData.hpp"
#include "app/Editor/EditorCommands.hpp"
#include "res/Terrain/TerrainSerialization.hpp"
#include "res/Resources.hpp"

namespace rpg::app {

struct EditorController
{
    gpu::EgoCamera camera;
    EditorCommands commands;

    EditorController()
    {
        camera.Update(0);
    }

    void Update(
    const double dt, 
    ecs::ECS& ecs, 
    res::Resources& res,
    gpu::RenderData& renderData)
    {
        if (app::glo::inputMode == app::glo::FlyMode)
            camera.Update(dt);
        if (wnd::glo::resizeState == wnd::glo::ResizeState::End)
            camera.UpdatePerspective();

        renderData.general.meta.view = camera.view;
        renderData.general.meta.proj = camera.perspective;
        renderData.general.meta.viewDir = com::Normalize(camera.rotation);

        //? serialization 

        if (wnd::HasEvent<wnd::EventType::F5, wnd::EventState::Pressed>())
        {
            ecs.WriteBinaryFile();
            res::SaveTerrain(res.terrain.terrain.quadrants);
        }

        if (wnd::HasEvent<wnd::EventType::F6, wnd::EventState::Pressed>())
        {
            ecs.ReadBinaryFile();
            res::LoadTerrain(res.terrain.terrain.quadrants);
            res.terrain.terrain.MarkAllDirty();
        }

        //? undo/redo

        commands.ExecuteDeferredCommands(ecs, res);

        if (wnd::HasEvent<wnd::EventType::Ctrl, wnd::EventState::PressedOrHeld>())
        {
            if (wnd::HasEvent<wnd::EventType::Z, wnd::EventState::Pressed>())
                commands.Undo(ecs, res);
            if (wnd::HasEvent<wnd::EventType::Y, wnd::EventState::Pressed>())
                commands.Redo(ecs, res);
        }



        auto& terrain = res.terrain.terrain;
        if (terrain.settings.mode == res::EditMode::PrefabPlacement)
        {
            if (const auto intersection = terrain.CheckIntersection(camera)) //more global to show gizmo
            {
                terrain.settings.intersectionPos = intersection->pos;
                if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Pressed>()) 
                {
                    const f32 RY = (f32) (std::rand() % 360);
                    const f32 SX = 0.8f + (std::rand() % 40) / 40.f;
                    const f32 SZ = 0.8f + (std::rand() % 40) / 40.f;
                    const f32 SY = 0.8f + (std::rand() % 40) / 40.f;

                    const app::EditorCommand cmd
                    {
                        .cmdEnum = app::EditorCommandEnum::CreateEntityFromPrefab,
                        .dataCreateEntityFromPrefab = 
                        {
                            .prefabEnum = terrain.settings.prefabEnum, //no need to save in terrain settings but grab from ui
                            .position   = intersection->pos,
                            .rotation   = {  0, RY,  0 },
                        }
                    };

                    commands.DeferCommand(cmd);
                }
            }
        }   


    }

};

}//ns