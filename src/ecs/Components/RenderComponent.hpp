//https://github.com/awwdev

#pragma once
#include "ecs/Components/Meta/ComponentEnum.hpp"
#include "res/Models/ModelMaps.hpp"
#include "dbg/Logger.hpp"

namespace rpg::ecs {

struct RenderComponent
{
    res::ModelType modelType;
    idx_t meshIdx;

    //defaults
    RenderComponent(const res::ModelType& pModelType = {}, const idx_t pMeshIdx = {})
        : modelType { pModelType }
        , meshIdx   { pMeshIdx   }
    {}

    //parsing
    RenderComponent(const ComponentDataStringPairs& pairs) : RenderComponent()
    {
        FOR_ARRAY(pairs, i) {
            const auto& pair = pairs[i];
            //const auto dataType = GetComponentDataType(pair.key);           
            //switch(dataType)
            {
                //case ComponentDataEnum::modelType:  
                //modelType = res::GetModelType(pair.val);  
                //break;             

                //default: dbg::LogWarning("component data type not defined for this component");
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