//https://github.com/awwdev

#pragma once
#include "app/InputMode.hpp"

#include "gpu/Meta/Camera.hpp"
#include "gpu/RenderData/RenderData.hpp"

#include "res/Resources.hpp"
#include "res/Terrain/TerrainSerialization.hpp"

#include "ecs/ECS.hpp"
#include "ecs/ECS_Serialization.hpp"

#include "app/Editor/EditorCommands.hpp"
#include "app/Editor/EditorMode.hpp"

namespace rpg::app {

struct Editor
{
    gpu::Camera    camera;
    EditorCommands commands;
    EditorMode     editorMode = EditorMode::TerrainVertexPaint;

    void Update(const double dt, ecs::ECS& ecs, res::Resources& res, gpu::RenderData& renderData)
    {
        //? camera
        if (app::glo::inputMode == app::glo::InputMode::FlyMode)
            camera.Update(dt);
        if (wnd::glo::resizeState == wnd::glo::ResizeState::End) 
            camera.UpdateProjection();
        camera.UpdateRays();
        camera.UpdateRenderData(renderData);

        //? serialization
        if (wnd::HasEvent<wnd::EventType::F5, wnd::EventState::Pressed>()) {
            ecs::SaveECS(ecs);
            res::SaveTerrain(res.terrain);     
        }
        if (wnd::HasEvent<wnd::EventType::F6, wnd::EventState::Pressed>()) {
            ecs::LoadECS(ecs);
            res::LoadTerrain(res.terrain); 
        }

        //? undo redo
        if (wnd::HasEvent<wnd::EventType::Ctrl, wnd::EventState::PressedOrHeld>())
        {
            if (wnd::HasEvent<wnd::EventType::Z, wnd::EventState::Pressed>())
                commands.Undo(ecs, res);
            if (wnd::HasEvent<wnd::EventType::Y, wnd::EventState::Pressed>())
                commands.Redo(ecs, res);
        }

        //? editor
        switch(editorMode)
        {
            case EditorMode::TerrainVertexPaint: TerrainVertexPaint(res.terrain); break;
            case EditorMode::TerrainVertexMove:  TerrainVertexMove(res.terrain); break;
            case EditorMode::PrefabPlacement:    PrefabPlacement(res, ecs); break;
            default: break;
        }
    }

    //? editor mode

    void TerrainVertexMove(res::Resources_Terrain& resTerrain)
    {
    }

    void TerrainVertexPaint(res::Resources_Terrain& resTerrain)
    {
        if (!wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::PressedOrHeld>())
            return;

        //TODO: command
        //TODO: falloff

        auto& terrain = resTerrain.terrain;
        if (auto const intersection = terrain.RayIntersection(camera.mouseRay))
        {
            auto& vertex = terrain.GetVertexByIntersection(*intersection);
            vertex.col = { 1, 0, 0, 1 };
        }
    }

    void PrefabPlacement(res::Resources& res, ecs::ECS& ecs)
    {
    }

};

}//ns




/*
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
*/



/*
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
*/