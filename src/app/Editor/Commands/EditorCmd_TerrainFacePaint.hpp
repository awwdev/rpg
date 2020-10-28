//https://github.com/awwdev

#pragma once
#include "ecs/ECS.hpp"
#include "res/Resources.hpp"
#include "app/Editor/EditorBrush.hpp"
#include "app/Editor/EditorCommandDirection.hpp"

namespace rpg::app {

struct EditorCmd_TerrainFacePaint
{
    idx_t affected_quadrantId;
    com::SimpleArray<idx_t, 100> affected_triangleIds; 
    com::SimpleArray<com::Vec4f, 100> dst_colors;
    com::SimpleArray<com::Vec4f, 100> ini_colors;

    template<EditorCommandDirection DIR>
    void Execute(res::Resources& res, ecs::ECS& ecs)
    {
        FOR_SIMPLE_ARRAY(affected_triangleIds, i)
        {
            auto const& triangleId = affected_triangleIds[i];
            auto& triangleColor = res.terrain.terrain.GetTriangleColor(affected_quadrantId, triangleId);
            if constexpr(DIR == EditorCommandDirection::Backwards) triangleColor = ini_colors[i];
            if constexpr(DIR == EditorCommandDirection::Forwards)  triangleColor = dst_colors[i];    
        }
    }
};

} //ns