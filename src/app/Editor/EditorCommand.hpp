//https://github.com/awwdev

#pragma once
#include "app/Editor/Commands/Cmd_CreateEntity.hpp"
#include "app/Editor/Commands/Cmd_TerrainEdit.hpp"

#include "ecs/ECS.hpp"
#include "res/Resources.hpp"

namespace rpg::app {

enum class EditorCommandEnum
{
    CreateEntityFromPrefab,
    TerrainVertexGrab,
};

struct EditorCommand
{
    EditorCommandEnum cmdEnum;

    Cmd_CreateEntityFromPrefab cmd_createEntityFromPrefab;
    Cmd_TerrainVertexGrab      cmd_terrainVertexGrab;


    void Execute(ecs::ECS& ecs, res::Resources& res)
    {
        switch(cmdEnum)
        {
            case app::EditorCommandEnum::CreateEntityFromPrefab: 
            cmd_createEntityFromPrefab.Execute(ecs); 
            break;

            case app::EditorCommandEnum::TerrainVertexGrab: 
            cmd_terrainVertexGrab.Execute(res.terrain); 
            break;

            default: dbg::Assert(false, "editor command missing");
        }
    }

    void ExecuteReverse(ecs::ECS& ecs, res::Resources& res)
    {
        switch(cmdEnum)
        {
            case app::EditorCommandEnum::CreateEntityFromPrefab: 
            cmd_createEntityFromPrefab.ExecuteReverse(ecs); 
            break;

            case app::EditorCommandEnum::TerrainVertexGrab: 
            cmd_terrainVertexGrab.ExecuteReverse(res.terrain); 
            break;

            default: dbg::Assert(false, "editor command missing");
        }
    }
};

} //ns