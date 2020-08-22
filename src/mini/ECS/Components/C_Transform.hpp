//https://github.com/awwdev

#pragma once
#include "mini/ECS/Components/Meta/ComponentTypes.hpp"
#include "mini/Utils/Matrix.hpp"
#include "mini/Debug/Logger.hpp"

namespace mini::ecs {

struct C_Transform
{
    utils::Mat4f transform;
    utils::Vec3f scale; 

    C_Transform(
        const utils::Mat4f& pTransform = {}, 
        const utils::Vec3f pScale = {})
        : transform { pTransform }
        , scale     { pScale }
    {}

    C_Transform(const ComponentDataStringPairs& pairs)
    {
        FOR_ARRAY(pairs, i) {
            //dbg::LogInfo(pairs[i].keyView, pairs[i].valView);
            const auto& pair = pairs[i];
            const ComponentDataType* dataType = COMPONENTDATA_STR_TO_ENUM.GetOptional(pair.key.data);
            dbg::Assert(dataType, "no component data type found");
            
            switch(*dataType)
            {
                case ComponentDataType::scale: scale = ParseComponentData<utils::Vec3f>(pair.val); break;
                default: dbg::LogWarning("component data type not defined for this component");
            }

            utils::PrintMatrix(scale);
        }
    }

};

}//NS