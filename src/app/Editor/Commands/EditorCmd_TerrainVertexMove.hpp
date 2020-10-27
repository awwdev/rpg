//https://github.com/awwdev

#pragma once
#include "res/Resources.hpp"
#include "res/Resources_Terrain.hpp"
#include "wnd/WindowEvents.hpp"
#include "app/Editor/EditorCommandDirection.hpp"

namespace rpg::app {

struct EditorCmd_TerrainVertexMove
{
    idx_t affected_quadrantIdx;
    com::SimpleArray<idx_t, 100> affected_vertexIds; 
    com::SimpleArray<com::Vec3f, 100> end_positions;
    com::SimpleArray<com::Vec3f, 100> beg_positions;

    template<EditorCommandDirection DIR>
    void Execute(res::Resources& res, ecs::ECS& ecs)
    {
        auto& terrain = res.terrain.terrain;
        auto& quadrant = terrain.quadrants[affected_quadrantIdx];
        FOR_SIMPLE_ARRAY(affected_vertexIds, i)
        {
            auto const& vertexId = affected_vertexIds[i];
            auto& vertex = quadrant.mesh.vertices[vertexId];

            if constexpr(DIR == EditorCommandDirection::Backwards) vertex.pos = beg_positions[i];
            if constexpr(DIR == EditorCommandDirection::Forwards)  vertex.pos = end_positions[i];    
        }
        quadrant.mesh.Recalculate();
        terrain.Stich(affected_quadrantIdx, affected_vertexIds);
    }
};

} //ns