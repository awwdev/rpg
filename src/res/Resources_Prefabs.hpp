//https://github.com/awwdev

#pragma once
#include "ecs/ComponentsMeta/ComponentArrays.hpp"
#include "res/Prefab/PrefabLoader.hpp"

#include "res/Mesh/MeshEnum.hpp"
#include "res/Mesh/MeshMaterialEnum.hpp"
#include "com/box/EnumMap.hpp"

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