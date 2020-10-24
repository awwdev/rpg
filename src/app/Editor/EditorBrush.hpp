//https://github.com/awwdev

#pragma once
#include "ecs/ECS.hpp"
#include "wnd/WindowEvents.hpp"
#include "res/Terrain/TerrainMesh.hpp"
#include "app/InputMode.hpp" //! should not be inside brush

namespace rpg::app {

struct EditorBrush
{
    //mesh
    ecs::ID brushID;

    //input
    com::Vec3f position;
    float scale = 1.f;
    float scaleSpeed = 0.1f;

    //falloff
    struct VertexWeight 
    { 
        res::TerrainVertex* vertexPtr;
        float weight;
    };
    com::Array<VertexWeight, 100> verticesInsideBrush;

    void UpdateScroll(ecs::ECS& ecs, float const dt)
    {
        if (wnd::HasEvent<wnd::EventType::Mouse_Scroll>())
        {
            auto& gizmoMainComponent = ecs.arrays.mainComponents.Get(brushID);
            scale -= wnd::glo::mouse_scroll_delta * scaleSpeed * dt;
            gizmoMainComponent.scale = com::InitializeWith<com::Vec3f>(scale);
        }
    }

    void CreateEntity(ecs::ECS& ecs)
    {
        brushID = ecs.AddEntity(res::PrefabEnum::Circle);
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
                const auto weight = scale / dist; //try other easing 
                verticesInsideBrush.AppendElement(&vertices[i], weight);
            }
        }
    }

};

} //ns