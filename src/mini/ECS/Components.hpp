//https://github.com/awwdev

#pragma once
#include "mini/Math/Matrix.hpp"
#include "mini/Box/EnumMap.hpp"
#include "mini/Box/StringMap.hpp"
#include "mini/Box/String.hpp"
#include "mini/Utils/CharsView.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/Resources/Mesh.hpp"

namespace mini::ecs
{
    //? WORKFLOW:
    //1) add enum
    //2) add string 
    //3) add struct
    //4) add to ComponentArrays
    //5) add parsing (prefabs)

    enum class ComponentType
    {
        Transform, 
        RenderData,
        ENUM_END
    };

    //?PARSING

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

    const box::StringMap<ComponentData> componentDataToStr
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
        //WARN("str to enum: invalid component type");
        return ComponentType::ENUM_END;
    }

    inline ComponentData GetComponentDataType(const utils::CharsView& view)
    {
        const auto* const  value = componentDataToStr.GetOptional(view);
        if (value) return *value;
        WARN("str to enum: invalid component data type");
        return ComponentData::ENUM_END;
    }

    //? COMPONENT STRUCTS

    struct C_Transform
    {
        math::Mat4f transform;
        //math::Vec3f pos;
        //rotation
        //scale
    };

    struct C_RenderData
    {
        res::MeshType meshType;
    };

}//ns