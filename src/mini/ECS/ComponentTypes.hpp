//https://github.com/awwdev

#pragma once
#include "mini/Box/StringMap.hpp"
#include "mini/Debug/Assert.hpp"

namespace mini::ecs {

constexpr auto COMPONENT_DATA_COUNT_MAX = 20; //how many data entries (for parsing)
constexpr auto COMPONENT_MAX_STR_LEN = 100;
constexpr auto IDENTIFIER_START = 2;

using ComponentDataString  = box::String<COMPONENT_MAX_STR_LEN>;
using ComponentDataStringArray = box::Array<ComponentDataString, ecs::COMPONENT_DATA_COUNT_MAX>;

//? COMPONENT TYPE PARSING

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


//? COMPONENT DATA TYPE PARSING

enum class ComponentDataType
{
    transform,
    meshType,
    ENUM_END
};

const box::EnumMap<ComponentDataType::ENUM_END, box::String<COMPONENT_MAX_STR_LEN>> COMPONENTDATA_ENUM_TO_STR
{
    { ComponentDataType::transform, "transform" },
    { ComponentDataType::meshType,  "meshType" },
};
const auto COMPONENTDATA_STR_TO_ENUM = box::StringMapFromEnumMap<ComponentDataType, COMPONENT_MAX_STR_LEN>(COMPONENTDATA_ENUM_TO_STR);

inline ComponentDataType GetComponentDataType(const ComponentDataString& str)
{
    using namespace ecs;
    
    const char* beg = &str[IDENTIFIER_START];
    const char* end = [&, end = beg]() mutable {
        while(*end != '=' && std::distance(beg, end) < 100)
            ++end;
        dbg::Assert(std::distance(beg, end) < 100, "component data has no assignment operator");
        return end;
    }();

    dbg::LogInfo(*beg, *end);;

    char key [100] = "";

    const auto* componentDataType = COMPONENTDATA_STR_TO_ENUM.GetOptional(key);
    dbg::Assert(componentDataType != nullptr, "componentDataType not found in map");


    return {};
}

}//NS