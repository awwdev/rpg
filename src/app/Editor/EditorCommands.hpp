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
    //com::Array     <EditorCommand, 10> deferredCmds; //a command should be singular, but I leave it as multiple for now 
    com::RingBuffer<EditorCommand, 20> cmdHistory;

    void StoreCommand(EditorCommand const& cmd)
    {
        cmdHistory.Append(cmd);
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