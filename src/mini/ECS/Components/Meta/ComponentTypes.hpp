//https://github.com/awwdev

#pragma once
#include "mini/Box/StringMap.hpp"
#include "mini/Debug/Assert.hpp"
#include "mini/ECS/Components/Meta/ComponentParsing.hpp"

namespace mini::ecs {

enum class ComponentType
{
    Transform, 
    RenderData,
    ENUM_END
};

const box::EnumMap<ComponentType::ENUM_END, box::String<COMPONENT_MAX_STR_LEN>> COMPONENT_ENUM_TO_STR
{
    { ComponentType::Transform,  "Transform" },
    { ComponentType::RenderData, "RenderData" },
};
const auto COMPONENT_STR_TO_ENUM = box::StringMapFromEnumMap<ComponentType, COMPONENT_MAX_STR_LEN>(COMPONENT_ENUM_TO_STR);


enum class ComponentDataType
{
    scale,
    meshType,
    useTexture,
    ENUM_END
};

const box::EnumMap<ComponentDataType::ENUM_END, box::String<COMPONENT_MAX_STR_LEN>> COMPONENTDATA_ENUM_TO_STR
{
    { ComponentDataType::scale,         "scale" },
    { ComponentDataType::meshType,      "meshType" },
    { ComponentDataType::useTexture,    "useTexture" },
};
const auto COMPONENTDATA_STR_TO_ENUM = box::StringMapFromEnumMap<ComponentDataType, COMPONENT_MAX_STR_LEN>(COMPONENTDATA_ENUM_TO_STR);

inline ComponentDataType GetComponentDataType(const ComponentDataString& str)
{
    const ComponentDataType* dataType = COMPONENTDATA_STR_TO_ENUM.GetOptional(str.data);
    dbg::Assert(dataType, "no component data type found");
    return *dataType;
}

}//NS