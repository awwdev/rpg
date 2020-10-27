//https://github.com/awwdev

#pragma once
#include "com/box/Array.hpp"
#include "com/box/RingBuffer.hpp"
#include "app/Editor/EditorCommand.hpp"
#include "app/Editor/EditorCommandDirection.hpp"

namespace rpg::app {

struct EditorCommands
{
    com::RingBuffer<EditorCommand, 50> cmdHistory;
    
    template<typename T>
    void ExecuteAndStoreCommand(T const& cmd, res::Resources& res, ecs::ECS& ecs)
    {
        auto editorCmd = CreateEditorCommand(cmd);
        editorCmd.template Execute<EditorCommandDirection::Forwards>(res, ecs);
        cmdHistory.Append(editorCmd);
    }

    void Undo(res::Resources& res, ecs::ECS& ecs)
    {
        if (auto* cmd = cmdHistory.StepBackwards())
            cmd->Execute<EditorCommandDirection::Backwards>(res, ecs);
    }

    void Redo(res::Resources& res, ecs::ECS& ecs)
    {
        if (auto* cmd = cmdHistory.StepForwards())
            cmd->Execute<EditorCommandDirection::Forwards>(res, ecs);
    }
};

} //ns