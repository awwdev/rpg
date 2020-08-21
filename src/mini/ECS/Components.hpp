//https://github.com/awwdev

#pragma once
#include "mini/Utils/Matrix.hpp"
#include "mini/Box/StringMap.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/Resources/MeshTypes.hpp"

namespace mini::ecs {

enum class ComponentType
{
    Transform, 
    RenderData,
    ENUM_END
};

constexpr auto COMPONENT_DATA_COUNT_MAX = 20; //how many data entries (for parsing)
constexpr auto COMPONENT_MAX_STR_LEN = 100;

const box::EnumMap<ComponentType::ENUM_END, box::String<COMPONENT_MAX_STR_LEN>> COMPONENT_ENUM_TO_STR
{
    { ComponentType::Transform, "Transform" },
    { ComponentType::RenderData, "RenderData" },
};
const auto COMPONENT_STR_TO_ENUM = box::StringMapFromEnumMap<ComponentType, COMPONENT_MAX_STR_LEN>(COMPONENT_ENUM_TO_STR);






    //?PARSING
/*
    const box::EnumMap<ComponentType::ENUM_END, box::String<20>> componentTypeToStr
    {
        { ComponentType::Transform,     "Transform"   },
        { ComponentType::RenderData,    "RenderData"  },
    };

    enum class ComponentData
    {
        pos,
        text,
        text_col,
        box,
        box_col,
        type,
        state,
        ENUM_END
    };

    const box::StringMap<ComponentData, 100> componentDataToStr
    {
        { "pos"   ,     ComponentData::pos          },
        { "box"   ,     ComponentData::box          },
        { "box_col"   , ComponentData::box_col      },
        { "text"  ,     ComponentData::text         },
        { "text_col"  , ComponentData::text_col     },
        { "type"  ,     ComponentData::type         },
        { "state" ,     ComponentData::state        },
    };

    inline ComponentType GetComponentType(const utils::CharsView& view)
    {
        //TODO: using a more efficient iteration
        //FOR_INDEX_MAP_BEGIN(componentTypeToStr, i)
        //    if (utils::CharsCompare(view, componentTypeToStr.Get(i).dataPtr))
        //        return (ComponentType)i;
        //FOR_INDEX_MAP_END
        //dbg::LogWarning("str to enum: invalid component type");
        return ComponentType::ENUM_END;
    }

    inline ComponentData GetComponentDataType(const utils::CharsView& view)
    {
        const auto* const  value = componentDataToStr.GetOptional(view);
        if (value) return *value;
        dbg::LogWarning("str to enum: invalid component data type");
        return ComponentData::ENUM_END;
    }
*/

    //? COMPONENT STRUCTS

    struct C_Transform
    {
        utils::Mat4f transform;
        //math::Vec3f pos;
        //rotation
        //scale
    };

    struct C_RenderData
    {
        res::MeshType meshType;
    };

}//ns