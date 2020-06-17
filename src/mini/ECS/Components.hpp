//https://github.com/awwdev

#pragma once
#include "mini/Math/Matrix.hpp"
#include "mini/Box/IndexMap.hpp"
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

    const box::IndexMap<box::String<20>, ComponentData::ENUM_END> componentDataToStr
    {
        { ComponentData::pos ,         "pos"   },
        { ComponentData::data1 ,       "data1" },
        { ComponentData::data2 ,       "data2" },
    };

    inline ComponentData GetComponentDataType(const utils::CharsView& view)
    {
        FOR_INDEX_MAP_BEGIN(componentDataToStr, i)
            if (utils::CharsCompare(view, componentDataToStr.Get(i).dataPtr))
                return (ComponentData)i;
        FOR_INDEX_MAP_END
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