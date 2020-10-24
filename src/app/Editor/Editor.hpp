//https://github.com/awwdev

#pragma once
#include "app/InputMode.hpp"
#include "com/utils/Utils.hpp"

#include "gpu/Meta/Camera.hpp"
#include "gpu/RenderData/RenderData.hpp"

#include "res/Resources.hpp"
#include "res/Terrain/TerrainSerialization.hpp"
#include "res/Prefab/PrefabEnum.hpp"

#include "ecs/ECS.hpp"
#include "ecs/ECS_Serialization.hpp"

#include "app/Editor/EditorCommands.hpp"
#include "app/Editor/EditorMode.hpp"
#include "app/Editor/EditorBrush.hpp"


namespace rpg::app {

struct Editor
{
    //? meta
    gpu::Camera    camera;
    EditorCommands commands;
    EditorMode     editorMode = EditorMode::TerrainVertexMove;
    EditorBrush    brush;
    res::PrefabEnum prefabEnum;

    void CreateGizmos(ecs::ECS& ecs)
    {
        brush.CreateEntity(ecs);
    }

    void Update(const double dt, ecs::ECS& ecs, res::Resources& res, gpu::RenderData& renderData)
    {
        UpdateCamera(dt, renderData);
        UpdateSerialization(ecs, res);
        UpdateCommands(ecs, res);
        UpdateEditing(dt, ecs, res);
    }

    //? camera
    void UpdateCamera(const double dt, gpu::RenderData& renderData)
    {
        if (app::glo::inputMode == app::glo::InputMode::FlyMode)
            camera.Update(dt);
        if (wnd::glo::resizeState == wnd::glo::ResizeState::End) 
            camera.UpdateProjection();
        camera.UpdateRays();
        camera.UpdateRenderData(renderData);
    }

    //? serialization
    void UpdateSerialization(ecs::ECS& ecs, res::Resources& res)
    {
        if (wnd::HasEvent<wnd::EventType::F5, wnd::EventState::Pressed>()) {
            ecs::SaveECS(ecs);
            res::SaveTerrain(res.terrain);     
        }
        if (wnd::HasEvent<wnd::EventType::F6, wnd::EventState::Pressed>()) {
            ecs::LoadECS(ecs);
            res::LoadTerrain(res.terrain); 
        }
    }

    //? commands
    void UpdateCommands(ecs::ECS& ecs, res::Resources& res)
    {   
        if (wnd::HasEvent<wnd::EventType::Ctrl, wnd::EventState::PressedOrHeld>())
        {
            if (wnd::HasEvent<wnd::EventType::Z, wnd::EventState::Pressed>())
                commands.Undo(ecs, res);
            if (wnd::HasEvent<wnd::EventType::Y, wnd::EventState::Pressed>())
                commands.Redo(ecs, res);
        }
    }

    //? editing
    void UpdateEditing(const double dt, ecs::ECS& ecs, res::Resources& res)
    {
        if (wnd::HasEvent<wnd::EventType::F3, wnd::EventState::Pressed>())
            editorMode = com::ScrollEnum(editorMode);

        brush.SetVisible(ecs, app::glo::inputMode == app::glo::InputMode::EditMode);
        if (app::glo::inputMode != app::glo::InputMode::EditMode) 
            return;

        brush.UpdateScroll(ecs, dt);
        auto const terrainIntersection = res.terrain.terrain.RayIntersection(camera.mouseRay);
        if (!terrainIntersection.HasValue()) 
            return;

        auto& vertices = res.terrain.terrain.quadrants[terrainIntersection->quadrantIdx].mesh.vertices;
        brush.SetPosition(ecs, terrainIntersection->point);
        brush.UpdateVerticesInsideBrush(vertices);

        switch(editorMode)
        {
            //? vertex paint
            case EditorMode::TerrainVertexPaint:
            {
                if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::PressedOrHeld>()) 
                {
                    FOR_ARRAY(brush.verticesInsideBrush, i)
                    {
                        auto const& vertexWeight = brush.verticesInsideBrush[i].weight;
                        auto& vertex = *brush.verticesInsideBrush[i].vertexPtr;
                        auto const colorBlended = com::InterpolateColors(vertex.col, brush.color, vertexWeight);
                        vertex.col = colorBlended;
                    }
                }
            }
            break;

            //? vertex move
            case EditorMode::TerrainVertexMove: 
            {
                if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Pressed>()) 
                    brush.vertexMoveYRef = wnd::glo::mouse_screen_y;

                if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::PressedOrHeld>()) 
                {
                    //TODO: this needs to only allow vertical movement
                    FOR_ARRAY(brush.verticesInsideBrush, i)
                    {
                        auto const& vertexWeight = brush.verticesInsideBrush[i].weight;
                        auto& vertex = *brush.verticesInsideBrush[i].vertexPtr;
                        vertex.pos.y -= wnd::glo::mouse_dy * brush.vertexMoveSpeed;
                    }
                }
            }
            break;

            //? prefab placement
            case EditorMode::PrefabPlacement: 
            {
                //TODO: get prefab from ui
            }
            break;

            default: break;
        }
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