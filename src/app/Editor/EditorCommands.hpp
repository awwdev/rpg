//https://github.com/awwdev

#pragma once
#include "com/Utils.hpp"
#include "com/box/Array.hpp"
#include "com/box/RingBuffer.hpp"
#include "ecs/ECS.hpp"

#include "app/Editor/Commands/Cmd_CreateEntity.hpp"
#include "app/Editor/Commands/Cmd_TerrainEdit.hpp"


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

    void Execute(ecs::ECS& ecs) const
    {
        switch(cmdEnum)
        {
            case app::EditorCommandEnum::CreateEntityFromPrefab: 
                dataCreateEntityFromPrefab.Execute(ecs); 
                break;
        }
    }

    void ExecuteReverse(ecs::ECS& ecs) const
    {
        switch(cmdEnum)
        {
            case app::EditorCommandEnum::CreateEntityFromPrefab: 
                dataCreateEntityFromPrefab.ExecuteReverse(ecs); 
                break;
        }
    }
};


struct EditorCommands
{
    com::Array<EditorCommand, 20> deferredCmds;
    com::RingBuffer<EditorCommand, 20> cmdHistory;

    void DeferCommand(EditorCommand const& cmd)
    {
        deferredCmds.AppendElement(cmd);
    }

    void ExecuteDeferredCommands(ecs::ECS& ecs)
    {
        FOR_ARRAY(deferredCmds, i)
        {
            auto const& cmd = deferredCmds[i];
            cmdHistory.Append(cmd);
            cmd.Execute(ecs);
        }
    }

    void Undo(ecs::ECS& ecs)
    {
        auto const& cmd = cmdHistory.StepBackward();
        cmd.ExecuteReverse(ecs);
    }

    void Redo(ecs::ECS& ecs)
    {
        auto const& cmd = cmdHistory.StepForward();
        cmd.ExecuteReverse(ecs);
    }

};

} //ns