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
    com::Array<EditorCommand, 20> deferredCmds;
    com::RingBuffer<EditorCommand, 20> cmdHistory;

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
        auto& cmd = cmdHistory.StepBackward();
        cmd.ExecuteReverse(ecs, res);
    }

    void Redo(ecs::ECS& ecs, res::Resources& res)
    {
        auto& cmd = cmdHistory.StepForward();
        cmd.Execute(ecs, res);
    }

};

} //ns