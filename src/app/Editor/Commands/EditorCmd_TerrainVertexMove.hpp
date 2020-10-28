//https://github.com/awwdev

#pragma once
#include "res/Resources.hpp"
#include "res/Terrain/TerrainStiching.hpp"
#include "app/Editor/EditorCommandDirection.hpp"

namespace rpg::app {

struct EditorCmd_TerrainVertexMove
{
    idx_t affected_quadrantId;
    com::SimpleArray<idx_t, 100> affected_vertexIds; 
    com::SimpleArray<com::Vec3f, 100> dst_positions;
    com::SimpleArray<com::Vec3f, 100> ini_positions;

    template<EditorCommandDirection DIR>
    void Execute(res::Resources& res, ecs::ECS& ecs)
    {
        FOR_SIMPLE_ARRAY(affected_vertexIds, i)
        {
            auto const& vertexId = affected_vertexIds[i];
            auto& vertex = res.terrain.terrain.GetVertex(affected_quadrantId, vertexId);
            if constexpr(DIR == EditorCommandDirection::Backwards) vertex.pos = ini_positions[i];
            if constexpr(DIR == EditorCommandDirection::Forwards)  vertex.pos = dst_positions[i];    
        }
        res::StichTerrain(res.terrain.terrain, affected_quadrantId);
    }
};

} //ns