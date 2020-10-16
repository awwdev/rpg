//https://github.com/awwdev

#pragma once
#include "ecs/ECS.hpp"
#include "com/Matrix.hpp"
#include "wnd/WindowEvents.hpp"
#include "gpu/Meta/Cameras.hpp"
#include "app/InputMode.hpp"
#include "gpu/RenderData/RenderData.hpp"
#include "app/Editor/EditorCommands.hpp"
#include "res/Terrain/_Old/TerrainSerialization.hpp"
#include "res/Resources.hpp"

namespace rpg::app {

struct EditorController
{
    gpu::EgoCamera camera;
    EditorCommands commands;

    void Update(const double dt, ecs::ECS& ecs, res::Resources& res, gpu::RenderData& renderData)
    {
        InputCamera(dt, renderData);
        Serialization(ecs, res);
        EditorCommands(ecs, res);
        PrefabPlacement(res, ecs);
        TerrainVertexGrab(res, ecs);
    }


    void InputCamera(const double dt, gpu::RenderData& renderData)
    {
        if (app::glo::inputMode == app::glo::FlyMode)
            camera.Update(dt);
        if (wnd::glo::resizeState == wnd::glo::ResizeState::End)
            camera.UpdatePerspective();

        camera.UpdateRenderData(renderData);
    }


    void Serialization(ecs::ECS& ecs, res::Resources& res)
    {
        if (wnd::HasEvent<wnd::EventType::F5, wnd::EventState::Pressed>())
        {
            ecs.WriteBinaryFile();
            res::WriteBinaryFile(res.terrain.terrain.quadrants);
        }

        if (wnd::HasEvent<wnd::EventType::F6, wnd::EventState::Pressed>())
        {
            ecs.ReadBinaryFile();
            res::ReadBinaryFile(res.terrain.terrain.quadrants);
            res.terrain.terrain.MarkAllDirty();
        }
    }


    void EditorCommands(ecs::ECS& ecs, res::Resources& res)
    {
        if (wnd::HasEvent<wnd::EventType::Ctrl, wnd::EventState::PressedOrHeld>())
        {
            if (wnd::HasEvent<wnd::EventType::Z, wnd::EventState::Pressed>())
                commands.Undo(ecs, res);
            if (wnd::HasEvent<wnd::EventType::Y, wnd::EventState::Pressed>())
                commands.Redo(ecs, res);
        }
    }


    void PrefabPlacement(res::Resources& res, ecs::ECS& ecs)
    {
        auto& terrain = res.terrain.terrain;
        if (terrain.settings.mode != res::EditMode::PrefabPlacement) //TODO: should be in controller
            return;

        if (const auto intersection = terrain.CheckIntersection(camera)) //more global to show gizmo
        {
            terrain.settings.intersectionPos = intersection->pos;
            if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Pressed>()) 
            {
                const f32 RY = (f32) (std::rand() % 360);
                const f32 SX = 0.8f + (std::rand() % 40) / 40.f;
                const f32 SZ = 0.8f + (std::rand() % 40) / 40.f;
                const f32 SY = 0.8f + (std::rand() % 40) / 40.f;

                app::EditorCommand cmd
                {
                    .cmdEnum = app::EditorCommandEnum::CreateEntityFromPrefab,
                    .cmd_createEntityFromPrefab = 
                    {
                        .prefabEnum = terrain.settings.prefabEnum, //no need to save in terrain settings but grab from ui
                        .position   = intersection->pos,
                        .rotation   = {  0, RY,  0 },
                    }
                };

                cmd.cmd_createEntityFromPrefab.Execute(ecs);
                commands.StoreCommand(cmd);
            }
        }
    }

    void TerrainVertexGrab(res::Resources& res, ecs::ECS& ecs)
    {
        auto& terrain = res.terrain.terrain;
        if (terrain.settings.mode != res::EditMode::VertexGrab) //TODO: should be in controller
            return;

        if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Released>())
        {
            auto const& editingVertIndices = terrain.settings.editingVertIndices;
            app::EditorCommand cmd
            {
                .cmdEnum = app::EditorCommandEnum::TerrainVertexGrab,
                .cmd_terrainVertexGrab = 
                {
                    .yGrabDist = terrain.settings.yGrabDist,
                }
            };
            cmd.cmd_terrainVertexGrab.editingVertIndices.AppendArray(editingVertIndices);

            //cmd.cmd_terrainVertexGrab.Execute(res.terrain);
            //do not execute (since it is alread done by terrain)
            commands.StoreCommand(cmd);
        }
    }

};

}//ns