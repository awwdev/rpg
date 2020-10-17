
//https://github.com/awwdev

#pragma once
#include "ecs/ECS.hpp"
#include <fstream>

namespace rpg::ecs {

void SaveECS(ecs::ECS const& ecs) 
{
    ecs.entitiesParentless.WriteBinaryFile("out/tmp/entitiesTopLevel.ecs");
    ecs.entities.WriteBinaryFile("out/tmp/entities.ecs");
    ecs.arrays.WriteBinaryFile();
}

void LoadECS(ecs::ECS& ecs)
{
    ecs.entitiesParentless.ReadBinaryFile("out/tmp/entitiesTopLevel.ecs");
    ecs.entities.ReadBinaryFile("out/tmp/entities.ecs");
    ecs.arrays.ReadBinaryFile();
}

}//ns