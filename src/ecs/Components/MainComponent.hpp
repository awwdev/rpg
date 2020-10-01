//https://github.com/awwdev

#pragma once
#include "com/box/Array.hpp"
#include "ecs/ComponentsMeta/ComponentData.hpp"
#include "res/Models/MeshEnum.hpp"
#include "res/Models/MaterialEnum.hpp"

namespace rpg::ecs {

struct MainComponent
{
    static constexpr idx_t CHILDREN_MAX = 10;
    com::Array<ID, CHILDREN_MAX> children;
    res::MeshEnum meshEnum;
    res::MaterialEnum materialEnum;

    MainComponent(ecs2::ComponentDataPairs const& pairs)
    {
        FOR_ARRAY(pairs, i) {
            auto const& pair = pairs[i];
            dbg::LogInfo(pair.key.data, pair.val.data);
        }
    }
};

}//NS
