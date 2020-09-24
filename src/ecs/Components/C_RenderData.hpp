//https://github.com/awwdev

#pragma once
#include "ecs/Components/Meta/ComponentTypes.hpp"
#include "res/_Old/CpuResources.hpp"
#include "dbg/Logger.hpp"
#include "res/Models/ModelType.hpp"

namespace rpg::ecs {

struct C_RenderData
{
    res::ModelType modelType;

    //defaults
    C_RenderData(const res::ModelType& pModelType = {})
        : modelType { pModelType }
    {}

    //parsing
    C_RenderData(const ComponentDataStringPairs& pairs) : C_RenderData()
    {
        FOR_ARRAY(pairs, i) {
            const auto& pair = pairs[i];
            const auto dataType = GetComponentDataType(pair.key);           
            switch(dataType)
            {
                case ComponentDataType::modelType:  
                modelType = res::GetModelType(pair.val);  
                break;             

                default: dbg::LogWarning("component data type not defined for this component");
            }
        }
    }
};

}//NS

/*
case ComponentDataType::castShadow:  
castShadow = (bool)std::atoi(pair.val.data);  
break;
*/