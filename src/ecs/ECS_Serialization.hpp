
//https://github.com/awwdev

#pragma once
#include "ecs/ECS.hpp"
#include <fstream>

namespace rpg::ecs {

void SaveECS(ecs::ECS const& ecs) 
{
    ecs.entitiesParentless.WriteBinaryFile("tmp/entitiesTopLevel.ecs");
    ecs.entities.WriteBinaryFile("tmp/entities.ecs");
    ecs.arrays.WriteBinaryFile();
}

void LoadECS(ecs::ECS& ecs)
{
    ecs.entitiesParentless.ReadBinaryFile("tmp/entitiesTopLevel.ecs");
    ecs.entities.ReadBinaryFile("tmp/entities.ecs");
    ecs.arrays.ReadBinaryFile();
}

}//ns