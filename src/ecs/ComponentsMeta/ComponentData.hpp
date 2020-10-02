#pragma once
#include "com/box/StringMap.hpp"
#include "res/Prefabs/PrefabEnum.hpp"
#include "dbg/Assert.hpp"
#include "com/box/String.hpp"
#include "com/box/Array.hpp"
#include "com/Algorithms.hpp"

namespace rpg::ecs {

enum class ComponentDataEnum
{
    MainComponent_Children,
    MainComponent_Mesh,
    MainComponent_Material,
    ENUM_END
};

const com::EnumMap<ComponentDataEnum::ENUM_END, com::String<res::PREFAB_FILE_LINE_LEN_MAX>> COMPONENTDATA_ENUM_TO_STR
{
    { ComponentDataEnum::MainComponent_Children, "Children" },
    { ComponentDataEnum::MainComponent_Mesh,     "Mesh" },
    { ComponentDataEnum::MainComponent_Material, "Material" },
};
const auto COMPONENTDATA_STR_TO_ENUM = com::StringMapFromEnumMap<ComponentDataEnum, res::PREFAB_FILE_LINE_LEN_MAX>(COMPONENTDATA_ENUM_TO_STR);




struct ComponentDataPair
{
    com::String<res::PREFAB_FILE_LINE_LEN_MAX> key;
    com::String<res::PREFAB_FILE_LINE_LEN_MAX> val;
};

constexpr auto COMPONENT_DATA_PAIR_MAX = 10;
using ComponentDataPairs =  com::Array<ComponentDataPair, COMPONENT_DATA_PAIR_MAX>;

inline auto ConvertToComponentDataPair(chars_t line) -> const ComponentDataPair
{
    ComponentDataPair pair {};

    auto len = com::StrLen(line);

    idx_t idx = 0;
    while(line[idx++] != '=')
        dbg::Assert(line[idx] != 0, "invalid component data pair");

    pair.key.Append(&line[0]  , idx - 1);
    pair.val.Append(&line[idx], len - idx);

    return pair;
}

} //ns