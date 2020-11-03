//https://github.com/awwdev

#pragma once
#include "res/Prefab/ComponentParsing/ComponentData.hpp"
#include "com/box/String.hpp"

namespace rpg::ecs {

struct NameComponent
{
    com::String<30> name = "Entity";

    NameComponent() = default;
    NameComponent(chars_t nameStr, idx_t const nameLen)
        : name { nameStr, nameLen }
    {}

    NameComponent(res::ComponentDataPairs const& pairs)
    {
    }

};

}//ns