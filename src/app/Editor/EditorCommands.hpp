//https://github.com/awwdev

#pragma once
#include "com/box/Array.hpp"
#include "com/box/RingBuffer.hpp"
#include "app/Editor/EditorCommand.hpp"

namespace rpg::app {

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

    void Undo(ecs::ECS& ecs, res::Resources& res)
    {
        auto const& cmd = cmdHistory.StepBackward();
        cmd.ExecuteReverse(ecs, res);
    }

    void Redo(ecs::ECS& ecs)
    {
        auto const& cmd = cmdHistory.StepForward();
        cmd.ExecuteReverse(ecs, res);
    }

};

} //ns