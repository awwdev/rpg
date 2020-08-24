//https://github.com/awwdev

#pragma once
#include "mini/ECS/Components/Meta/ComponentTypes.hpp"
#include "mini/Resources/MeshTypes.hpp"
#include "mini/Debug/Logger.hpp"

namespace mini::ecs {

struct C_RenderData
{
    res::MeshType meshType;
    bool useTexture;

    //defaults
    C_RenderData(
        const res::MeshType& pMeshType = {},
        const bool pUseTexture = false)
        : meshType      { pMeshType   }
        , useTexture    { pUseTexture }
    {}

    //parsing
    C_RenderData(const ComponentDataStringPairs& pairs) : C_RenderData()
    {
        FOR_ARRAY(pairs, i) {
            const auto& pair = pairs[i];
            const auto dataType = GetComponentDataType(pair.key);           
            switch(dataType)
            {
                case ComponentDataType::meshType:  
                meshType = res::GetMeshType(pair.val);  
                break;

                case ComponentDataType::useTexture:  
                useTexture = (bool)std::atoi(pair.val.data);  
                break;

                default: dbg::LogWarning("component data type not defined for this component");
            }
        }
    }
};

}//NS