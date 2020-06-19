//https://github.com/awwdev

#pragma once
#include "mini/Math/Matrix.hpp"
#include "mini/Box/IndexMap.hpp"
#include "mini/Box/StringMap.hpp"
#include "mini/Box/String.hpp"
#include "mini/Utils/CharsView.hpp"

namespace mini::ecs
{
    enum class ComponentData
    {
        pos,
        data1,
        data2,
        ENUM_END
    };

    constexpr box::StringMap<ComponentData> componentDataToStr
    {
        { "pos"   , ComponentData::pos    },
        { "data1" , ComponentData::data1  },
        { "data2" , ComponentData::data2  },
    };

    inline ComponentData GetComponentDataType(const utils::CharsView& view)
    {
        const auto* const  value = componentDataToStr.GetOptional(view);
        if (value) return *value;
        WARN("str to enum: invalid component data type");
        return ComponentData::ENUM_END;
    }



    struct C_Transform
    {
        math::Vec3f pos;
        //rotation
        //scale
    };

    struct C_RenderData
    {

    };
    
}//ns