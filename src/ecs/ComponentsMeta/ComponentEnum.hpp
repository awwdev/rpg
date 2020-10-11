//https://github.com/awwdev

#pragma once
#include "ecs/EntityID.hpp"
#include "com/box/String.hpp"
#include "com/box/StringMap.hpp"
#include "res/Prefab/PrefabEnum.hpp"

namespace rpg::ecs {

enum class ComponentEnum : ecs::ID
{
    MainComponent, 
    NameComponent,
    ENUM_END
};

const com::EnumMap<ComponentEnum::ENUM_END, com::String<res::PREFAB_FILE_LINE_LEN_MAX>> COMPONENT_ENUM_TO_STR
{
    { ComponentEnum::MainComponent, "MainComponent" },
    { ComponentEnum::NameComponent, "NameComponent" },
};
const auto COMPONENT_STR_TO_ENUM = com::StringMapFromEnumMap<ComponentEnum, res::PREFAB_FILE_LINE_LEN_MAX>(COMPONENT_ENUM_TO_STR);



struct ComponentFiles
{
    ComponentFiles() = default;
    ComponentFiles(chars_t path)
        : dense { path }
        , componentLookup { path }
        , entityLookup { path }
    {
        dense.AppendArray(".dense");
        componentLookup.AppendArray(".componentLookup");
        entityLookup.AppendArray(".entityLookup");
    }

    com::String<100> dense;
    com::String<100> componentLookup;
    com::String<100> entityLookup;
};

const com::EnumMap<ComponentEnum::ENUM_END, ComponentFiles> COMPONENT_SERIALIZATION_PATHS
{
    { ComponentEnum::MainComponent, "res/ecs/MainComponent" },
    { ComponentEnum::NameComponent, "res/ecs/NameComponent" },
};

}//NS