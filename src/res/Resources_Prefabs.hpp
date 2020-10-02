//https://github.com/awwdev

#pragma once
#include "ecs/ComponentsMeta/ComponentArrays.hpp"
#include "res/Prefabs/PrefabLoader.hpp"

namespace rpg::res {

struct Resources_Prefabs
{
    ecs::ComponentArrays<res::PrefabEnum::ENUM_END> prefabsArrays;

    void Load()
    {
        LoadPrefabs("res/Prefabs/Prefabs.fab", prefabsArrays);
        dbg::LogInfo("prefabs loaded");
    }
};

}//ns