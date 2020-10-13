//https://github.com/awwdev

#pragma once
#include "com/Utils.hpp"
#include "com/box/Array.hpp"
#include "ecs/ECS.hpp"

#include "app/Editor/Commands/Cmd_CreateEntity.hpp"


namespace rpg::app {

enum class EditorCommandEnum
{
    CreateEntity,
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
                case app::EditorCommandEnum::CreateEntity: cmd.createEntity.Execute(ecs); break;
            }
        }
    }

};

} //ns