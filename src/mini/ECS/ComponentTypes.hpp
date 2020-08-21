//https://github.com/awwdev

#pragma once
#include "mini/Box/StringMap.hpp"
#include "mini/Debug/Assert.hpp"
#include "mini/Utils/CharsView.hpp"

namespace mini::ecs {

constexpr auto COMPONENT_DATA_COUNT_MAX = 20; //how many data entries (for parsing)
constexpr auto COMPONENT_MAX_STR_LEN = 100;
constexpr auto IDENTIFIER_START = 2;

using ComponentDataString  = box::String<COMPONENT_MAX_STR_LEN>;
using ComponentDataStringArray = box::Array<ComponentDataString, ecs::COMPONENT_DATA_COUNT_MAX>;

struct ComponentDataKeyValue
{
    utils::CharsView keyView;
    utils::CharsView valView;
};

using ComponentDataKeyValueArray = box::Array<ComponentDataKeyValue, ecs::COMPONENT_DATA_COUNT_MAX>;

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
    scale,
    meshType,
    ENUM_END
};

const box::EnumMap<ComponentDataType::ENUM_END, box::String<COMPONENT_MAX_STR_LEN>> COMPONENTDATA_ENUM_TO_STR
{
    { ComponentDataType::scale,     "scale" },
    { ComponentDataType::meshType,  "meshType" },
};
const auto COMPONENTDATA_STR_TO_ENUM = box::StringMapFromEnumMap<ComponentDataType, COMPONENT_MAX_STR_LEN>(COMPONENTDATA_ENUM_TO_STR);

inline const ComponentDataKeyValue GetComponentDataKeyValue(const ComponentDataString& str)
{
    using namespace ecs;

    const char* beg = &str.data[IDENTIFIER_START];
    const char* end = [&, end = beg]() mutable {
        while(*end != '=' && std::distance(beg, end) < COMPONENT_MAX_STR_LEN) 
            ++end;
        dbg::Assert(std::distance(beg, end) < COMPONENT_MAX_STR_LEN, "component data has no assignment operator");
        return end;
    }();
    const auto len1 = (idx_t)std::distance(beg, end) - 1;
    const auto len2 = (idx_t)std::distance(end, &str[str.Length()-1]) - 1;

    const ComponentDataKeyValue keyValue { { &str.data[IDENTIFIER_START], len1 }, { end + 2, len2 } };
    return keyValue;
}

template<class T>
inline T ParseComponentData(const utils::CharsView& view)
{
    T data {};

    if constexpr(std::is_same_v<T, utils::Vec3f>)
    {
        idx_t comma = 0;
        idx_t x = 0;
        for(idx_t i = 0; i < view.len && x < 3; ++i){
            if (view.beginPtr[i] == ','){
                data[0][x] = (f32)std::atof(view.beginPtr + comma);
                comma = i + 1;
                ++x;
            }  
        }
    }
    else
    {
        static_assert(false, "no impl yet");
    }

    return data;
}


}//NS