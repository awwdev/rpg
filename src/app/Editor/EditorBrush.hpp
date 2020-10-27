//https://github.com/awwdev

#pragma once
#include "ecs/ECS.hpp"
#include "wnd/WindowEvents.hpp"
#include "res/Terrain/TerrainMesh.hpp"
#include "res/Resources_Terrain.hpp"
#include "res/Prefab/PrefabEnum.hpp"
#include "com/box/SimpleArray.hpp"

namespace rpg::app {

struct EditorBrush
{
    //gizmo
    ecs::ID brushID;

    //input
    com::Vec3f position;
    float scale = 3.f;
    float scaleSpeed = 0.5f;

    //painting
    com::Vec4f color;
    float vertexMoveSpeed = 0.1f;
    res::PrefabEnum prefabEnum;

    //frequency
    float frequencyCounter = 0;
    float frequencyCounterMax = 0.03;
    bool Frequency(float const dt) 
    {
        frequencyCounter += dt;
        if (frequencyCounter >= frequencyCounterMax)
        {
            frequencyCounter -= frequencyCounterMax;
            return true;
        }
        return false;
    }

    struct VerticesInsideBrush
    {
        com::SimpleArray<idx_t, 100> vertexIds; 
        com::SimpleArray<float, 100> vertexWeights; //aligned 

        void Clear()
        {
            vertexIds.Clear();
            vertexWeights.Clear();
        }
    } 
    verticesInsideBrush;

    //? methods

    void CreateGizmo(ecs::ECS& ecs)
    {
        brushID = ecs.AddEntity(res::PrefabEnum::Circle);
        auto& gizmoMainComponent = ecs.arrays.mainComponents.Get(brushID);
        gizmoMainComponent.scale = com::InitializeWith<com::Vec3f>(scale); //initial scale
    }

    void UpdateGizmo(float const dt, ecs::ECS& ecs, com::Vec3f const& pPosition)
    {
        //size update
        if (wnd::HasEvent<wnd::EventType::Mouse_Scroll>())
        {
            auto& gizmoMainComponent = ecs.arrays.mainComponents.Get(brushID);
            scale -= wnd::glo::mouse_scroll_delta * scaleSpeed * dt;
            gizmoMainComponent.scale = com::InitializeWith<com::Vec3f>(scale);
        }

        //position update
        auto& gizmoMainComponent = ecs.arrays.mainComponents.Get(brushID);
        position = pPosition;
        gizmoMainComponent.translation = position;
    }

    void UpdateVerticesInsideBrush(res::Resources_Terrain& resTerrain, res::RayQuadrant_Intersection const& intersection)
    {
        verticesInsideBrush.Clear();

        //update vertex data inside brush
        auto& terrain  = resTerrain.terrain;
        auto& quadrant = terrain.quadrants[intersection.quadrantId];
        auto& meshVertices = quadrant.mesh.vertices;

        FOR_C_ARRAY(meshVertices, vertexId)
        {
            auto const& vertPos = meshVertices[vertexId].pos;
            auto const dist = com::Distance(vertPos, intersection.position);
            if (dist < scale)
            {
                const auto weight = 1 - dist / scale; //try other easing 
                verticesInsideBrush.vertexIds.Append(vertexId);
                verticesInsideBrush.vertexWeights.Append(weight);
            }
        }
    }

    void SetVisible(ecs::ECS& ecs, bool const set)
    {
        auto& gizmoMainComponent = ecs.arrays.mainComponents.Get(brushID);
        gizmoMainComponent.meshEnum = set ? res::MeshEnum::Circle : res::MeshEnum::None;
    }

};

} //ns