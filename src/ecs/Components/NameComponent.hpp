//https://github.com/awwdev

#pragma once
#include "ecs/ComponentsMeta/ComponentData.hpp"
#include "com/box/String.hpp"

namespace rpg::ecs {

struct NameComponent
{
    com::String<30> name = "Entity";

    NameComponent() = default;
    NameComponent(chars_t nameStr, idx_t const nameLen)
        : name { nameStr, nameLen }
    {}

    NameComponent(ecs::ComponentDataPairs const& pairs)
    {
    }

};

}//ns