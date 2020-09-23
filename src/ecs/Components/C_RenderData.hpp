//https://github.com/awwdev

#pragma once
#include "ecs/Components/Meta/ComponentTypes.hpp"
#include "res/_Old/CpuResources.hpp"
#include "dbg/Logger.hpp"
#include "res/_Old/MeshTypes.hpp"

namespace rpg::ecs {

struct C_RenderData
{
    res::MeshType meshType;
    bool useTexture;
    bool castShadow;

    //defaults
    C_RenderData(
        const res::MeshType& pMeshType  = {},
        const bool pUseTexture          = false,
        const bool pCastShadow          = true)
        : meshType      { pMeshType   }
        , useTexture    { pUseTexture }
        , castShadow    { pCastShadow }
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

                case ComponentDataType::castShadow:  
                castShadow = (bool)std::atoi(pair.val.data);  
                break;

                default: dbg::LogWarning("component data type not defined for this component");
            }
        }
    }
};

}//NS