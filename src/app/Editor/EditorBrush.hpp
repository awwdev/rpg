//https://github.com/awwdev

#pragma once
#include "ecs/ECS.hpp"
#include "wnd/WindowEvents.hpp"
#include "res/Terrain/TerrainMesh.hpp"
#include "res/Prefab/PrefabEnum.hpp"
#include "com/box/SimpleArray.hpp"

namespace rpg::app {

struct EditorBrush
{
    //?mesh
    ecs::ID brushID;

    //?input
    com::Vec3f position;
    float scale = 3.f;
    float scaleSpeed = 0.5f;

    //?vertex color
    com::Vec4f color;
    //?vertex move
    bool  vertexGrabbed = false;
    float vertexMoveSpeed = 0.1f;
    //?prefab placement
    res::PrefabEnum prefabEnum;

    //?frequency
    float frequencyCounter = 0;
    float frequencyCounterMax = 0.05;
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

    //?falloff
    struct VertexWeight 
    { 
        res::TerrainVertex* vertexPtr;
        float weight;
    };
    using BrushVertices = com::SimpleArray<VertexWeight, 100>;
    BrushVertices verticesInsideBrush;


    void CreateEntity(ecs::ECS& ecs)
    {
        brushID = ecs.AddEntity(res::PrefabEnum::Circle);
        auto& gizmoMainComponent = ecs.arrays.mainComponents.Get(brushID);
        gizmoMainComponent.scale = com::InitializeWith<com::Vec3f>(scale); //initial scale
    }

    void UpdateScale(ecs::ECS& ecs, float const dt)
    {
        if (wnd::HasEvent<wnd::EventType::Mouse_Scroll>())
        {
            auto& gizmoMainComponent = ecs.arrays.mainComponents.Get(brushID);
            scale -= wnd::glo::mouse_scroll_delta * scaleSpeed * dt;
            gizmoMainComponent.scale = com::InitializeWith<com::Vec3f>(scale);
        }
    }

    void SetVisible(ecs::ECS& ecs, bool const set)
    {
        auto& gizmoMainComponent = ecs.arrays.mainComponents.Get(brushID);
        gizmoMainComponent.meshEnum = set ? res::MeshEnum::Circle : res::MeshEnum::None;
    }

    void SetPosition(ecs::ECS& ecs, com::Vec3f const& pPosition)
    {
        auto& gizmoMainComponent = ecs.arrays.mainComponents.Get(brushID);
        position = pPosition;
        gizmoMainComponent.translation = position;
    }

    template<auto N>
    void UpdateVerticesInsideBrush(res::TerrainVertex (&vertices)[N]) 
    {
        verticesInsideBrush.Clear();
        FOR_C_ARRAY(vertices, i)
        {
            auto const& vertPos = vertices[i].pos;
            auto const dist = com::Distance(vertPos, position);
            if (dist < scale)
            {
                const auto weight = 1 - dist / scale; //try other easing 
                verticesInsideBrush.Append(&vertices[i], weight);
            }
        }
    }

};

} //ns