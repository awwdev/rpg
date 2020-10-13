//https://github.com/awwdev

#pragma once
#include "com/Utils.hpp"
#include "com/box/Array.hpp"
#include "ecs/ECS.hpp"

namespace rpg::app {

enum class EditorCommandEnum
{
    CreateEntity,
};

struct CmdCreateEntity
{
    res::PrefabEnum prefabEnum;
    com::Vec3f position;
    com::Vec3f rotation;
    com::Vec3f scale;
};

struct EditorCommand
{
    EditorCommandEnum cmdEnum;
    union 
    {
        CmdCreateEntity createEntity;
    };
};

struct EditorCommands
{
    com::Array<EditorCommand, 20> cmds;
    idx_t cmdExeIndex = 0;

    void DeferCommand(EditorCommand const& cmd)
    {
        cmds.AppendElement(cmd);
    }

    void ExecuteCommands(ecs::ECS& ecs)
    {
        for(; cmdExeIndex < cmds.Count(); ++cmdExeIndex)
        {
            auto const& cmd = cmds[cmdExeIndex];
            switch(cmd.cmdEnum)
            {
                case app::EditorCommandEnum::CreateEntity:
                {
                    auto const& cmdUnion = cmd.createEntity;
                    auto const entityID = ecs.AddEntity(cmdUnion.prefabEnum);
                    auto& mainComponent = ecs.arrays.mainComponents.Get(entityID);
                    mainComponent.translation = cmdUnion.position;
                    mainComponent.rotation = cmdUnion.rotation;
                    mainComponent.scale = cmdUnion.scale;
                }
                break;
            }
        }
    }

};

} //ns