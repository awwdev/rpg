//https://github.com/awwdev

#pragma once
#include "app/InputMode.hpp"
#include "com/utils/Utils.hpp"

#include "gpu/Meta/Camera.hpp"
#include "gpu/RenderData/RenderData.hpp"

#include "res/Resources.hpp"
#include "res/Terrain/TerrainSerialization.hpp"

#include "ecs/ECS.hpp"
#include "ecs/ECS_Serialization.hpp"

#include "app/Editor/EditorCommands.hpp"
#include "app/Editor/EditorMode.hpp"
#include "app/Editor/EditorBrush.hpp"


namespace rpg::app {

struct Editor
{
    gpu::Camera     camera;
    EditorCommands  commands;
    EditorBrush     brush;
    EditorMode      editorMode;

    void CreateGizmos(ecs::ECS& ecs)
    {
        brush.CreateGizmo(ecs);
    }

    void Update(const double dt, ecs::ECS& ecs, res::Resources& res, gpu::RenderData& renderData)
    {
        UpdateCamera(dt, renderData);
        UpdateSerialization(ecs, res);
        UpdateCommands(ecs, res);
        UpdateEditing(dt, ecs, res);
    }

    void UpdateCamera(const double dt, gpu::RenderData& renderData)
    {
        if (wnd::glo::resizeState == wnd::glo::ResizeState::End) 
            camera.UpdateProjection();

        if (editorMode.active == false)
        {
            camera.UpdateRays(); //will update terrain intersection
            if (app::glo::inputMode == app::glo::InputMode::FlyMode)
                camera.UpdateView(dt);
            camera.UpdateRenderData(renderData);
        }
    }

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

    void UpdateCommands(ecs::ECS& ecs, res::Resources& res)
    {   
        if (wnd::HasEvent<wnd::EventType::Ctrl, wnd::EventState::PressedOrHeld>())
        {
            if (wnd::HasEvent<wnd::EventType::Z, wnd::EventState::Pressed>())
                commands.Undo(res, ecs);
            if (wnd::HasEvent<wnd::EventType::Y, wnd::EventState::Pressed>())
                commands.Redo(res, ecs);
        }
    }

    void UpdateEditing(const double dt, ecs::ECS& ecs, res::Resources& res)
    {
        //brush visibility needs to be overwritten
        brush.SetVisible(ecs, app::glo::inputMode == app::glo::InputMode::EditMode); 

        if (app::glo::inputMode != app::glo::InputMode::EditMode) 
            return;

        //scroll editor editing mode
        if (wnd::HasEvent<wnd::EventType::F2, wnd::EventState::Pressed>())
            com::AdvanceEnum(editorMode.editorEnum);

        switch(editorMode.editorEnum)
        {
            case EditorEnum::TerrainVertexPaint: TerrainVertexPaint (dt, ecs, res); break;
            case EditorEnum::TerrainFacePaint:   TerrainFacePaint   (dt, ecs, res); break;
            case EditorEnum::TerrainVertexMove:  TerrainVertexMove  (dt, ecs, res); break;
            case EditorEnum::PrefabPlacement:    PrefabPlacement    (dt, ecs, res); break;
            default: break;
        }
    }

    void TerrainVertexPaint(float const dt, ecs::ECS& ecs, res::Resources& res)
    {
        auto& terrain = res.terrain.terrain;

        const auto terrainIntersection = terrain.RayIntersection(camera.mouseRay);
        if (terrainIntersection.HasValue() == false)
            return;

        brush.UpdateGizmo(dt, ecs, terrainIntersection->position);
        brush.UpdateInsideBrush(res.terrain, *terrainIntersection);

        if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::PressedOrHeld>() &&
            wnd::HasEvent<wnd::EventType::Mouse_Move>() && brush.Frequency(dt)) 
        {
            EditorCmd_TerrainVertexPaint cmd;
            cmd.affected_quadrantId = terrainIntersection->quadrantId;
            cmd.affected_vertexIds = brush.insideBrush.vertexIds;
            FOR_SIMPLE_ARRAY(brush.insideBrush.vertexIds, i)
            {
                auto const& vertexId = brush.insideBrush.vertexIds[i];
                auto& vertex = terrain.GetVertex(terrainIntersection->quadrantId, vertexId);
                cmd.ini_colors.Append(vertex.col);

                auto const& vertexWeight = brush.insideBrush.vertexWeights[i];
                auto const colorBlended = com::InterpolateColors(vertex.col, brush.color, vertexWeight);
                cmd.dst_colors.Append(colorBlended);
            }
            commands.ExecuteAndStoreCommand(cmd, res, ecs);
        }
    }

    void TerrainFacePaint(float const dt, ecs::ECS& ecs, res::Resources& res)
    {
        static idx_t triangleId = 0;
        auto& terrain = res.terrain.terrain;

        auto const terrainIntersection = terrain.RayIntersection(camera.mouseRay);
        if (terrainIntersection.HasValue() == false)
            return;

        brush.UpdateGizmo(dt, ecs, terrainIntersection->position);
        brush.UpdateInsideBrush(res.terrain, *terrainIntersection);

        if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::PressedOrHeld>() &&
            wnd::HasEvent<wnd::EventType::Mouse_Move>() && brush.Frequency(dt)) 
        {
            if (triangleId == terrainIntersection->quadrantTriangleId)
                return; //don't cpature multiple times
            triangleId = terrainIntersection->quadrantTriangleId;

            EditorCmd_TerrainFacePaint cmd;
            cmd.affected_quadrantId = terrainIntersection->quadrantId;
            cmd.affected_triangleIds.Append(terrainIntersection->quadrantTriangleId);
            //TODO: want to capture all triangle inside brush, currently it is only the intersection one
            auto& triangleColor = res.terrain.terrain.GetTriangleColor(terrainIntersection->quadrantId, terrainIntersection->quadrantTriangleId);
            cmd.ini_colors.Append(triangleColor);
            cmd.dst_colors.Append(brush.color);
            commands.ExecuteAndStoreCommand(cmd, res, ecs);
        }
    }

    void TerrainVertexMove(float const dt, ecs::ECS& ecs, res::Resources& res)
    {
        auto& terrain = res.terrain.terrain;
        static auto terrainIntersection = terrain.RayIntersection(camera.mouseRay);
        static EditorCmd_TerrainVertexMove cmd {};

        //? brush update (defer!)
        if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::PressedOrHeld>() == false &&
            editorMode.active == false)
        {
            terrainIntersection = res.terrain.terrain.RayIntersection(camera.mouseRay);
            if (terrainIntersection.HasValue() == false)
                return;
            brush.UpdateGizmo(dt, ecs, terrainIntersection->position);
            brush.UpdateInsideBrush(res.terrain, *terrainIntersection);
        } 

        if (terrainIntersection.HasValue() == false)
            return;

        //? intial state
        if (wnd::MouseLeftButtonPressed())
        {
            brush.SetVisible(ecs, false);
            editorMode.active = true;

            cmd = {}; //!reset
            cmd.affected_quadrantId = terrainIntersection->quadrantId;
            cmd.affected_vertexIds = brush.insideBrush.vertexIds;
            FOR_SIMPLE_ARRAY(brush.insideBrush.vertexIds, i)
            {
                auto const& vertexId = brush.insideBrush.vertexIds[i];
                auto& vertex = terrain.GetVertex(terrainIntersection->quadrantId, vertexId);
                cmd.ini_positions.Append(vertex.pos);
            }
        }

        //? live update (no cmd)
        if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::PressedOrHeld>()) 
        {
            brush.SetVisible(ecs, false);
            FOR_SIMPLE_ARRAY(brush.insideBrush.vertexIds, i)
            {
                auto const& vertexWeight = brush.insideBrush.vertexWeights[i];
                auto const& vertexId = brush.insideBrush.vertexIds[i];
                auto& vertex = terrain.GetVertex(terrainIntersection->quadrantId, vertexId);
                vertex.pos.y += wnd::glo::mouse_dy * (vertexWeight/2 + 0.5) * brush.vertexMoveSpeed;
            }
            terrain.quadrants[terrainIntersection->quadrantId].mesh.Recalculate();
        }

        //? destination state
        if (wnd::MouseLeftButtonReleased())
        {
            editorMode.active = false;
            FOR_SIMPLE_ARRAY(brush.insideBrush.vertexIds, i)
            {
                auto const& vertexId = brush.insideBrush.vertexIds[i];
                auto& vertex = terrain.GetVertex(terrainIntersection->quadrantId, vertexId);
                cmd.dst_positions.Append(vertex.pos);
            }
            commands.ExecuteAndStoreCommand(cmd, res, ecs); //this will also call the mesh update
        }

    }

    void PrefabPlacement(float const dt, ecs::ECS& ecs, res::Resources& res)
    {
        auto& terrain = res.terrain.terrain;
        const auto terrainIntersection = terrain.RayIntersection(camera.mouseRay);
        if (terrainIntersection.HasValue() == false)
            return;

        brush.UpdateGizmo(dt, ecs, terrainIntersection->position);
        brush.UpdateInsideBrush(res.terrain, *terrainIntersection);

        if (wnd::MouseLeftButtonPressed() || (
            wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Held>() && 
            wnd::HasEvent<wnd::EventType::Mouse_Move>() && brush.Frequency(dt)
            ))
        {
            EditorCmd_PrefabPlacement cmd { .prefabEnum = brush.prefabEnum, .position = terrainIntersection->position };
            commands.ExecuteAndStoreCommand(cmd, res, ecs);
        }
    }

};

}//ns