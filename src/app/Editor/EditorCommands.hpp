//https://github.com/awwdev

#pragma once
#include "com/box/Array.hpp"
#include "com/box/RingBuffer.hpp"
#include "app/Editor/EditorCommand.hpp"

namespace rpg::app {

struct EditorCommands
{
    com::RingBuffer<EditorCommand, 20> cmdHistory;

    void ExecuteCommand(EditorCommand const& pCmd, res::Resources& res, ecs::ECS& ecs)
    {
        auto& cmd = cmdHistory.Append(pCmd);
        cmd.Execute(res, ecs);
    }

    void Undo(res::Resources& res, ecs::ECS& ecs)
    {
        if (auto* cmd = cmdHistory.StepBackwards())
            cmd->ExecuteReverse(res, ecs);
    }

    void Redo(res::Resources& res, ecs::ECS& ecs)
    {
        if (auto* cmd = cmdHistory.StepForwards())
            cmd->Execute(res, ecs);
    }
};

} //ns