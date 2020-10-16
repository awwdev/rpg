//https://github.com/awwdev

#pragma once
#include "com/Utils.hpp"
#include "com/Matrix.hpp"
#include "com/box/Array.hpp"
#include "res/Resources_Terrain.hpp"
#include "ecs/ECS.hpp"

namespace rpg::app {

struct Cmd_TerrainVertexGrab
{
    float yGrabDist;
    com::Array<res::VertexBrushInfo, 100> editingVertIndices;

    void Execute(res::Resources_Terrain& resTerrain) const
    {
        /*
        auto& terrain = resTerrain.terrain;
        auto& quadrant = terrain.GetQuadrant(terrain.settings.quadrantIdx);
        FOR_ARRAY(editingVertIndices, i)
        {
            const auto idx     = editingVertIndices[i].idx;
            const auto falloff = editingVertIndices[i].falloff;
            quadrant.verts[idx].pos.y += yGrabDist * terrain.settings.dragScale * falloff;
            const auto triangleIdx = (idx / 3) * 3;
            quadrant.RecalculateNormalsOfTriangle(triangleIdx);
        }
        terrain.settings.dirtyQuadrants.AppendElement(terrain.settings.quadrantIdx);
        */
    }

    void ExecuteReverse(res::Resources_Terrain& resTerrain)
    {
        /*
        auto& terrain = resTerrain.terrain;
        auto& quadrant = terrain.GetQuadrant(terrain.settings.quadrantIdx);
        FOR_ARRAY(editingVertIndices, i)
        {
            const auto idx     = editingVertIndices[i].idx;
            const auto falloff = editingVertIndices[i].falloff;
            quadrant.verts[idx].pos.y -= yGrabDist * terrain.settings.dragScale * falloff;
            const auto triangleIdx = (idx / 3) * 3;
            quadrant.RecalculateNormalsOfTriangle(triangleIdx);
        }
        terrain.settings.dirtyQuadrants.AppendElement(terrain.settings.quadrantIdx);
        */
    }

};

}//ns