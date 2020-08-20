//https://github.com/awwdev

#pragma once
#include "mini/Box/StringMap.hpp"

namespace mini::ecs {

enum PrefabType
{
    Sword,
    Stone,
    Grass,
    ENUM_MAX
};

constexpr ID PREFAB_COUNT_MAX = (ID)PrefabType::ENUM_MAX;

const box::StringMap<PrefabType, 100> PREFAB_STRINGS
{
    { "Sword", PrefabType::Sword },
    { "Stone", PrefabType::Stone },
    { "Grass", PrefabType::Grass },
};

}//NS