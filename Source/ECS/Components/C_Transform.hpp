//https://github.com/awwdev

#pragma once
#include "ECS/Components/Meta/ComponentTypes.hpp"
#include "Common/Matrix.hpp"
#include "Debug/Logger.hpp"

namespace rpg::ecs {

struct C_Transform
{
    com::Vec3f scale; 
    com::Vec3f rotation; 
    com::Vec3f translation; 

    //defaults
    C_Transform( 
        const com::Vec3f pScale       = { 1, 1, 1 },
        const com::Vec3f pRotation    = { 0, 0, 0 },
        const com::Vec3f pTranslation = { 0, 0, 0 })
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
                scale = ParseComponentData<com::Vec3f>(pair.val); 
                break;

                default: dbg::LogWarning("component data type not defined for this component");
            }
        }
    }

};

}//NS