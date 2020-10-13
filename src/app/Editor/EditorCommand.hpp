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
};

struct EditorCommand
{
    EditorCommandEnum cmdEnum;
    union 
    {
        CmdCreateEntityFromPrefab dataCreateEntityFromPrefab;
    };

    void Execute(ecs::ECS& ecs, res::Resources& res) const
    {
        switch(cmdEnum)
        {
            case app::EditorCommandEnum::CreateEntityFromPrefab: 
                dataCreateEntityFromPrefab.Execute(ecs); 
                break;
        }
    }

    void ExecuteReverse(ecs::ECS& ecs, res::Resources& res) const
    {
        switch(cmdEnum)
        {
            case app::EditorCommandEnum::CreateEntityFromPrefab: 
                dataCreateEntityFromPrefab.ExecuteReverse(ecs); 
                break;
        }
    }
};

} //ns