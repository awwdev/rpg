//https://github.com/awwdev

#pragma once
#include "mini/ECS/Components/Meta/ComponentTypes.hpp"
#include "mini/Resources/MeshTypes.hpp"
#include "mini/Debug/Logger.hpp"

namespace mini::ecs {

struct C_RenderData
{
    res::MeshType meshType;

    C_RenderData(
        const res::MeshType& pMeshType = {})
        : meshType { pMeshType }
    {}

    C_RenderData(const ComponentDataStringPairs& pairs)
    {
        FOR_ARRAY(pairs, i) {
            const auto& pair = pairs[i];
            const auto dataType = GetComponentDataType(pair.key);           
            switch(dataType)
            {
                case ComponentDataType::meshType: 
                {
                    const auto meshType = res::GetMeshType(pair.val);  
                    dbg::LogInfo(res::MESHTYPE_ENUM_TO_STR.Get(meshType));
                }
                break;

                default: dbg::LogWarning("component data type not defined for this component");
            }
        }
    }
};

}//NS