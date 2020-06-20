//https://github.com/awwdev

#pragma once
#include "mini/Math/Matrix.hpp"
#include "mini/Box/IndexMap.hpp"
#include "mini/Box/StringMap.hpp"
#include "mini/Box/String.hpp"
#include "mini/Utils/CharsView.hpp"

namespace mini::ecs
{
    //? WORKFLOW:
    //1) add enum
    //2) add string 
    //3) add struct
    //4) add to ComponentArrays
    //5) add parsing (prefabs)

    enum ComponentType
    {
        Transform, 
        RenderData,
        UI,
        ENUM_END
    };

    const box::IndexMap<box::String<20>, ComponentType::ENUM_END> componentTypeToStr
    {
        { ComponentType::Transform,     "Transform"   },
        { ComponentType::RenderData,    "RenderData"  },
        { ComponentType::UI,            "UI"          },
    };

    enum class ComponentData
    {
        pos,
        data1,
        data2,
        ENUM_END
    };

    const box::StringMap<ComponentData> componentDataToStr
    {
        { "pos"   , ComponentData::pos    },
        { "data1" , ComponentData::data1  },
        { "data2" , ComponentData::data2  },
    };


    inline ComponentType GetComponentType(const utils::CharsView& view)
    {
        FOR_INDEX_MAP_BEGIN(componentTypeToStr, i)
            if (utils::CharsCompare(view, componentTypeToStr.Get(i).dataPtr))
                return (ComponentType)i;
        FOR_INDEX_MAP_END
        WARN("str to enum: invalid component type");
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
        math::Vec3f pos;
        //rotation
        //scale
    };

    struct C_RenderData
    {

    };

    struct C_UI
    {
        math::Vec2f pos;
    };
    
}//ns