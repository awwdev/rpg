#pragma once
#include "com/box/StringMap.hpp"
#include "ecs/Prefabs/PrefabEnum.hpp"
#include "dbg/Assert.hpp"
#include "com/box/String.hpp"
#include "com/Algorithms.hpp"

namespace rpg::ecs2 {

enum class ComponentDataEnum
{
    Children,
    Mesh,
    Material,
    ENUM_END
};

const com::EnumMap<ComponentDataEnum::ENUM_END, com::String<ecs2::PREFAB_FILE_LINE_LEN_MAX>> COMPONENTDATA_ENUM_TO_STR
{
    { ComponentDataEnum::Children,      "Children" },
    { ComponentDataEnum::Mesh,          "Mesh" },
    { ComponentDataEnum::Material,      "Material" },
};
const auto COMPONENTDATA_STR_TO_ENUM = com::StringMapFromEnumMap<ComponentDataEnum, ecs2::PREFAB_FILE_LINE_LEN_MAX>(COMPONENTDATA_ENUM_TO_STR);




struct ComponentDataPair
{
    com::String<PREFAB_FILE_LINE_LEN_MAX> key;
    com::String<PREFAB_FILE_LINE_LEN_MAX> val;
};

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