//https://github.com/awwdev

#pragma once
#include "com/box/Array.hpp"
#include "com/box/RingBuffer.hpp"

#include "app/Editor/Commands/EditorCmd_Base.hpp"
#include "app/Editor/Commands/EditorCmd_PrefabPlacement.hpp"
#include "app/Editor/Commands/EditorCmd_TerrainFacePaint.hpp"
#include "app/Editor/Commands/EditorCmd_TerrainVertexMove.hpp"
#include "app/Editor/Commands/EditorCmd_TerrainVertexPaint.hpp"

#include "com/mem/Allocator.hpp"

namespace rpg::app {

struct EditorCommands
{
    com::RingBuffer<com::mem::BlockPtr<EditorCmd_Base>, 20> cmdHistory;

    void StoreCommand()
    {
        //cmdHistory.Append(cmd);
    }

    void Undo(ecs::ECS& ecs, res::Resources& res)
    {
        //if (auto* cmd = cmdHistory.StepBackwards())
        //    cmd->ExecuteReverse(ecs, res);
    }

    void Redo(ecs::ECS& ecs, res::Resources& res)
    {
        //if (auto* cmd = cmdHistory.StepForwards())
        //    cmd->Execute(ecs, res);
    }

};

} //ns