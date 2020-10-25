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
    //? meta
    gpu::Camera     camera;
    EditorCommands  commands;
    EditorMode      editorMode = EditorMode::TerrainVertexMove;
    EditorBrush     brush;

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

    void UpdateCamera(const double dt, gpu::RenderData& renderData)
    {
        if (app::glo::inputMode == app::glo::InputMode::FlyMode)
            camera.Update(dt);
        if (wnd::glo::resizeState == wnd::glo::ResizeState::End) 
            camera.UpdateProjection();
        camera.UpdateRays();
        camera.UpdateRenderData(renderData);
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
                commands.Undo(ecs, res);
            if (wnd::HasEvent<wnd::EventType::Y, wnd::EventState::Pressed>())
                commands.Redo(ecs, res);
        }
    }

    void UpdateEditing(const double dt, ecs::ECS& ecs, res::Resources& res)
    {
        brush.SetVisible(ecs, app::glo::inputMode == app::glo::InputMode::EditMode);

        if (wnd::HasEvent<wnd::EventType::F3, wnd::EventState::Pressed>())
            res.terrain.terrain.StichTerrain();

        //reset
        if (wnd::MouseLeftButtonReleased())
        {
            brush.vertexGrabbed = false;
        }

        if (app::glo::inputMode != app::glo::InputMode::EditMode) 
            return;

        //scroll editor editing mode
        if (wnd::HasEvent<wnd::EventType::F2, wnd::EventState::Pressed>())
            editorMode = com::ScrollEnum(editorMode);

        auto const terrainIntersection = res.terrain.terrain.RayIntersection(camera.mouseRay);
        if (!terrainIntersection.HasValue()) 
            return;

        switch(editorMode)
        {
            case EditorMode::TerrainVertexPaint: TerrainVertexPaint (dt, ecs, res, *terrainIntersection); break;
            case EditorMode::TerrainFacePaint:   TerrainFacePaint   (dt, ecs, res, *terrainIntersection); break;
            case EditorMode::TerrainVertexMove:  TerrainVertexMove  (dt, ecs, res, *terrainIntersection); break;
            case EditorMode::PrefabPlacement:    PrefabPlacement    (dt, ecs, res, *terrainIntersection); break;
            default: break;
        }
    }

    void TerrainVertexPaint(float const dt, ecs::ECS& ecs, res::Resources& res, res::RayQuadrant_Intersection const& terrainIntersection)
    {
        brush.SetPosition(ecs, terrainIntersection.point);
        brush.UpdateScale(ecs, dt);
        auto& vertices = res.terrain.terrain.quadrants[terrainIntersection.quadrantIdx].mesh.vertices;
        brush.UpdateVerticesInsideBrush(vertices);

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

    void TerrainFacePaint(float const dt, ecs::ECS& ecs, res::Resources& res, res::RayQuadrant_Intersection const& terrainIntersection)
    {
        brush.SetPosition(ecs, terrainIntersection.point);
        brush.UpdateScale(ecs, dt);
        auto& vertices = res.terrain.terrain.quadrants[terrainIntersection.quadrantIdx].mesh.vertices;
        brush.UpdateVerticesInsideBrush(vertices);

        if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::PressedOrHeld>()) 
        {
            auto const& triangleIdx = terrainIntersection.quadrantTriangleIdx;
            auto& quadrant = res.terrain.terrain.quadrants[terrainIntersection.quadrantIdx];
            quadrant.mesh.triangleColors[triangleIdx] = brush.color;
        }
    }

    void TerrainVertexMove(float const dt, ecs::ECS& ecs, res::Resources& res, res::RayQuadrant_Intersection const& terrainIntersection)
    {
        brush.SetVisible(ecs, true);
        if (wnd::MouseLeftButtonPressed()) 
        {
            brush.vertexGrabbed = true;
        }
        //reset is done in editor update begin

        if (brush.vertexGrabbed == false)
        {
            brush.UpdateScale(ecs, dt);
            brush.SetPosition(ecs, terrainIntersection.point);
            auto& vertices = res.terrain.terrain.quadrants[terrainIntersection.quadrantIdx].mesh.vertices;
            brush.UpdateVerticesInsideBrush(vertices);
        }
        else
        {
            brush.SetVisible(ecs, false);
            FOR_ARRAY(brush.verticesInsideBrush, i)
            {
                auto const& vertexWeight = brush.verticesInsideBrush[i].weight;
                auto& vertex = *brush.verticesInsideBrush[i].vertexPtr;
                vertex.pos.y += wnd::glo::mouse_dy * (vertexWeight/2 + 0.5) * brush.vertexMoveSpeed;
                //easing
            }
            //recalculate mesh
            auto& quadrant = res.terrain.terrain.quadrants[terrainIntersection.quadrantIdx];
            quadrant.mesh.Recalculate();
        }
    }

    void PrefabPlacement(float const dt, ecs::ECS& ecs, res::Resources& res, res::RayQuadrant_Intersection const& terrainIntersection)
    {
        brush.SetPosition(ecs, terrainIntersection.point);
        brush.UpdateScale(ecs, dt);
        if (wnd::MouseLeftButtonPressed()) 
        {
            auto const& prefabEnum = brush.prefabEnum;
            auto const& placementPoint = terrainIntersection.point;
            auto const id = ecs.AddEntity(prefabEnum);
            auto& mainComponent = ecs.arrays.mainComponents.Get(id);
            mainComponent.translation = placementPoint;
        }
    }

};

}//ns