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

            switch(cmd.cmdEnum)
            {
                case app::EditorCommandEnum::CreateEntityFromPrefab: 
                cmd.dataCreateEntityFromPrefab.Execute(ecs); break;
            }
        }
    }

    void Undo()
    {
        //TODO
        //cmdHistory
    }

    void Redo()
    {
        //TODO
        //cmdHistory
    }

};

} //ns