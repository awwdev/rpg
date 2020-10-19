//https://github.com/awwdev

#pragma once
#include "app/InputMode.hpp"
#include "gpu/Meta/Cameras.hpp"
#include "ecs/ECS.hpp"
#include "res/Resources.hpp"
#include "gpu/RenderData/RenderData.hpp"

#include "res/Terrain/TerrainSerialization.hpp"
#include "ecs/ECS_Serialization.hpp"

#include "app/Editor/EditorCommands.hpp"
#include "app/Editor/EditorMode.hpp"

namespace rpg::app {

struct Editor
{
    gpu::EgoCamera camera;
    EditorCommands commands;
    EditorMode     editorMode = EditorMode::TerrainVertexPaint;

    //ray test
    com::Ray ray {
        .origin { 0, -4, 0 },
        .length { 0,  5, 0 }, 
    };
    ecs::MainComponent* rayMainComponent;


    void Update(const double dt, ecs::ECS& ecs, res::Resources& res, gpu::RenderData& renderData)
    {
        //ray test
        static bool rayInit = false;
        if (!rayInit)
        {
            rayInit = true;
            auto const id = ecs.AddEntity(res::PrefabEnum::Cube);
            rayMainComponent = &ecs.arrays.mainComponents.Get(id);
            rayMainComponent->scale = { 0.02, 3, 0.02 };
        }

        InputCamera(dt, renderData);
        Serialization(ecs, res);
        UndoRedo(ecs, res);

        switch(editorMode)
        {
            case EditorMode::TerrainVertexMove:  TerrainVertexMove  (res.terrain); break;
            case EditorMode::TerrainVertexPaint: TerrainVertexPaint (res.terrain); break;
            case EditorMode::PrefabPlacement:    PrefabPlacement    (res.terrain); break;
            default: break;
        }
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
        if (wnd::HasEvent<wnd::EventType::F5, wnd::EventState::Pressed>()) {
            ecs::SaveECS(ecs);
            res2::SaveTerrain();     
        }

        if (wnd::HasEvent<wnd::EventType::F6, wnd::EventState::Pressed>()) {
            ecs::LoadECS(ecs);
            res2::LoadTerrain(); 
        }
    }

    void UndoRedo(ecs::ECS& ecs, res::Resources& res)
    {
        if (wnd::HasEvent<wnd::EventType::Ctrl, wnd::EventState::PressedOrHeld>())
        {
            if (wnd::HasEvent<wnd::EventType::Z, wnd::EventState::Pressed>())
                commands.Undo(ecs, res);
            if (wnd::HasEvent<wnd::EventType::Y, wnd::EventState::Pressed>())
                commands.Redo(ecs, res);
        }
    }

    //? editor mode

    void TerrainVertexMove(res::Resources_Terrain& resTerrain)
    {
    }

    void TerrainVertexPaint(res::Resources_Terrain& resTerrain)
    {
        //ray test
        if(wnd::HasEvent<wnd::EventType::U, wnd::EventState::PressedOrHeld>())
            ray.origin.y -= 0.02;
        if(wnd::HasEvent<wnd::EventType::J, wnd::EventState::PressedOrHeld>())
            ray.origin.y += 0.02;
        if(wnd::HasEvent<wnd::EventType::T, wnd::EventState::PressedOrHeld>())
            ray.origin.z += 0.02;
        if(wnd::HasEvent<wnd::EventType::G, wnd::EventState::PressedOrHeld>())
            ray.origin.z -= 0.02;
        if(wnd::HasEvent<wnd::EventType::F, wnd::EventState::PressedOrHeld>())
            ray.origin.x -= 0.02;
        if(wnd::HasEvent<wnd::EventType::H, wnd::EventState::PressedOrHeld>())
            ray.origin.x += 0.02;

        rayMainComponent->translation = ray.origin;

        auto& terrain = resTerrain.terrain;
        if (auto const intersectionPoint = terrain.RayIntersection(ray))
        {
            com::PrintMatrix(*intersectionPoint);
        }
    }

    void PrefabPlacement(res::Resources_Terrain& resTerrain)
    {
    }








    


   


    


    void TerrainVertexGrab(res::Resources_Terrain& resTerrain, ecs::ECS& ecs)
    {
        auto& terrain = resTerrain.terrain;






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
    }










    void PrefabPlacement(res::Resources& res, ecs::ECS& ecs)
    {
        auto& terrain = res.terrain.terrain;
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
    }

    

};

}//ns