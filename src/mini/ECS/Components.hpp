//https://github.com/awwdev

#pragma once
#include "mini/Box/StringMap.hpp"
#include "mini/ECS/Components/C_RenderData.hpp"
#include "mini/ECS/Components/C_Transform.hpp"

namespace mini::ecs {

enum class ComponentType
{
    Transform, 
    RenderData,
    ENUM_END
};

//? PARSING
constexpr auto COMPONENT_DATA_COUNT_MAX = 20; //how many data entries (for parsing)
constexpr auto COMPONENT_MAX_STR_LEN = 100;
using ComponentDataStrings = box::Array<box::String<COMPONENT_MAX_STR_LEN>, ecs::COMPONENT_DATA_COUNT_MAX>;

const box::EnumMap<ComponentType::ENUM_END, box::String<COMPONENT_MAX_STR_LEN>> COMPONENT_ENUM_TO_STR
{
    { ComponentType::Transform, "Transform" },
    { ComponentType::RenderData, "RenderData" },
};
const auto COMPONENT_STR_TO_ENUM = box::StringMapFromEnumMap<ComponentType, COMPONENT_MAX_STR_LEN>(COMPONENT_ENUM_TO_STR);

}//NS