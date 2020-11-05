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

    struct InsideBrush
    {
        com::SimpleArray<idx_t, 100> vertexIds; 
        com::SimpleArray<idx_t, 100> triangleIds; 
        com::SimpleArray<float, 100> vertexWeights; //aligned 

        void Clear()
        {
            vertexIds.Clear();
            triangleIds.Clear();
            vertexWeights.Clear();
        }
    } 
    insideBrush;

    //? methods

    void CreateGizmo(ecs::ECS& ecs)
    {
        brushID = ecs.AddEntity(res::PrefabEnum::Circle);
        auto& transform_component = ecs.arrays.transform_components.Get(brushID);
        transform_component.scale = com::InitializeWith<com::Vec3f>(scale); //initial scale
    }

    void UpdateGizmo(float const dt, ecs::ECS& ecs, com::Vec3f const& pPosition)
    {
        //size update
        auto& transform_component = ecs.arrays.transform_components.Get(brushID);

        //position update
        position = pPosition;
        transform_component.translation = position;

        //scale
        if (wnd::HasEvent<wnd::EventType::Mouse_Scroll>())
        {
            scale -= wnd::glo::mouse_scroll_delta * scaleSpeed * dt;
            transform_component.scale = com::InitializeWith<com::Vec3f>(scale);
        }
    }

    void UpdateInsideBrush(res::Resources_Terrain& resTerrain, res::RayQuadrant_Intersection const& intersection)
    {
        insideBrush.Clear();

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
                insideBrush.vertexIds.Append(vertexId);
                insideBrush.vertexWeights.Append(weight);
            }
        }
    }

    void SetVisible(ecs::ECS& ecs, bool const set)
    {
        auto& render_component = ecs.arrays.render_components.Get(brushID);
        render_component.meshEnum = set ? res::MeshEnum::Circle : res::MeshEnum::None;
    }

};

} //ns