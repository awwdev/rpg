//https://github.com/awwdev

#pragma once
#include "com/box/StringMap.hpp"
#include "dbg/Assert.hpp"
#include "ecs/Components/Meta/ComponentParsing.hpp"

namespace rpg::ecs {

enum class ComponentType
{
    Transform, 
    RenderData,
    ENUM_END
};

const com::EnumMap<ComponentType::ENUM_END, com::String<COMPONENT_MAX_STR_LEN>> COMPONENT_ENUM_TO_STR
{
    { ComponentType::Transform,  "Transform" },
    { ComponentType::RenderData, "RenderData" },
};
const auto COMPONENT_STR_TO_ENUM = com::StringMapFromEnumMap<ComponentType, COMPONENT_MAX_STR_LEN>(COMPONENT_ENUM_TO_STR);


enum class ComponentDataType
{
    scale,
    modelType,
    ENUM_END
};

const com::EnumMap<ComponentDataType::ENUM_END, com::String<COMPONENT_MAX_STR_LEN>> COMPONENTDATA_ENUM_TO_STR
{
    { ComponentDataType::scale,         "scale" },
    { ComponentDataType::modelType,     "modelType" },
};
const auto COMPONENTDATA_STR_TO_ENUM = com::StringMapFromEnumMap<ComponentDataType, COMPONENT_MAX_STR_LEN>(COMPONENTDATA_ENUM_TO_STR);

inline ComponentDataType GetComponentDataType(const ComponentDataString& str)
{
    const ComponentDataType* dataType = COMPONENTDATA_STR_TO_ENUM.GetOptional(str.data);
    dbg::Assert(dataType, "no component data type found");
    return *dataType;
}

}//NS