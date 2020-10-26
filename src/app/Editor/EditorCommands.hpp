//https://github.com/awwdev

#pragma once
#include "com/box/Array.hpp"
#include "com/box/RingBuffer.hpp"
#include "app/Editor/EditorCommand.hpp"

namespace rpg::app {

struct EditorCommands
{
    com::RingBuffer<EditorCommand, 50> cmdHistory;

    template<class T>
    void ExecuteAndAStoreCommand(T& cmd, res::Resources& res, ecs::ECS& ecs)
    {
        cmd.Execute(res, ecs);
        cmdHistory.Append(CreateEditorCommand(cmd));
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