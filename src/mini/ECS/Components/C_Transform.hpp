//https://github.com/awwdev

#pragma once
#include "mini/ECS/Components/Meta/ComponentTypes.hpp"
#include "mini/Utils/Matrix.hpp"
#include "mini/Debug/Logger.hpp"

namespace mini::ecs {

struct C_Transform
{
    utils::Vec3f scale; 
    utils::Vec3f rotation; 
    utils::Vec3f translation; 

    //defaults
    C_Transform( 
        const utils::Vec3f pScale       = { 1, 1, 1 },
        const utils::Vec3f pRotation    = { 0, 0, 0 },
        const utils::Vec3f pTranslation = { 0, 0, 0 })
        : scale        { pScale }
        , rotation     { pRotation }
        , translation  { pTranslation }
    {}

    //parsing
    C_Transform(const ComponentDataStringPairs& pairs) : C_Transform()
    {
        FOR_ARRAY(pairs, i) {
            const auto& pair = pairs[i];
            const auto dataType = GetComponentDataType(pair.key);      
            switch(dataType)
            {
                case ComponentDataType::scale: 
                scale = ParseComponentData<utils::Vec3f>(pair.val); 
                break;

                default: dbg::LogWarning("component data type not defined for this component");
            }
        }
    }

};

}//NS