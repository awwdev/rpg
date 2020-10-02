//https://github.com/awwdev

#pragma once
#include "ecs/ComponentsMeta/ComponentArrays.hpp"
#include "res/Prefab/PrefabLoader.hpp"

namespace rpg::res {

struct Resources_Prefabs
{
    ecs::ComponentArrays<res::PrefabEnum::ENUM_END> prefabsArrays;

    void Load()
    {
        prefabsArrays.Clear();
        LoadPrefabs("res/Prefabs/Prefabs.fab", prefabsArrays);
    }
};

}//ns