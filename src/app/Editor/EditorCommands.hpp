//https://github.com/awwdev

#pragma once
#include "com/box/Array.hpp"
#include "com/box/RingBuffer.hpp"
#include "app/Editor/EditorCommand.hpp"

#include "ecs/ECS.hpp"
#include "res/Resources.hpp"

namespace rpg::app {

struct EditorCommands
{
    com::Array<EditorCommand, 10> deferredCmds;
    com::RingBuffer<EditorCommand, 10> cmdHistory;

    void DeferCommand(EditorCommand const& cmd)
    {
        deferredCmds.AppendElement(cmd);
    }

    void ExecuteDeferredCommands(ecs::ECS& ecs, res::Resources& res)
    {
        FOR_ARRAY(deferredCmds, i)
        {
            auto& cmd = deferredCmds[i];
            cmd.Execute(ecs, res);
            cmdHistory.Append(cmd);
        }
        deferredCmds.Clear();
    }

    void Undo(ecs::ECS& ecs, res::Resources& res)
    {
        if (auto* cmd = cmdHistory.StepBackwards())
            cmd->ExecuteReverse(ecs, res);
    }

    void Redo(ecs::ECS& ecs, res::Resources& res)
    {
        if (auto* cmd = cmdHistory.StepForwards())
            cmd->Execute(ecs, res);
    }

};

} //ns