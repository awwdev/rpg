//https://github.com/awwdev

#pragma once
#include "app/Editor/Commands/EditorCmd_PrefabPlacement.hpp"
#include "app/Editor/Commands/EditorCmd_TerrainFacePaint.hpp"
#include "app/Editor/Commands/EditorCmd_TerrainVertexMove.hpp"
#include "app/Editor/Commands/EditorCmd_TerrainVertexPaint.hpp"

namespace rpg::app {

enum class EditorCommandEnum
{
    EditorCmd_PrefabPlacement,
    EditorCmd_TerrainFacePaint,
    EditorCmd_TerrainVertexMove,
    EditorCmd_TerrainVertexPaint
};

struct EditorCommand
{
    EditorCommandEnum editorCommandEnum;
    union 
    {
        EditorCmd_PrefabPlacement    cmd_prefabPlacement;
        EditorCmd_TerrainFacePaint   cmd_terrainFacePaint;
        EditorCmd_TerrainVertexMove  cmd_terrainVertexMove;
        EditorCmd_TerrainVertexPaint cmd_terrainVertexPaint;
    };
    

    void Execute(res::Resources& res, ecs::ECS& ecs)
    {
        switch(editorCommandEnum)
        {
            case EditorCommandEnum::EditorCmd_PrefabPlacement:     cmd_prefabPlacement.Execute(res, ecs); break;
            case EditorCommandEnum::EditorCmd_TerrainFacePaint:    cmd_terrainFacePaint.Execute(res, ecs); break;
            case EditorCommandEnum::EditorCmd_TerrainVertexMove:   cmd_terrainVertexMove.Execute(res, ecs); break;
            case EditorCommandEnum::EditorCmd_TerrainVertexPaint:  cmd_terrainVertexPaint.Execute(res, ecs); break;
        }
    }

    void ExecuteReverse(res::Resources& res, ecs::ECS& ecs)
    {
        switch(editorCommandEnum)
        {
            case EditorCommandEnum::EditorCmd_PrefabPlacement:     cmd_prefabPlacement.ExecuteReverse(res, ecs); break;
            case EditorCommandEnum::EditorCmd_TerrainFacePaint:    cmd_terrainFacePaint.ExecuteReverse(res, ecs); break;
            case EditorCommandEnum::EditorCmd_TerrainVertexMove:   cmd_terrainVertexMove.ExecuteReverse(res, ecs); break;
            case EditorCommandEnum::EditorCmd_TerrainVertexPaint:  cmd_terrainVertexPaint.ExecuteReverse(res, ecs); break;
        }
    }
};

}//ns