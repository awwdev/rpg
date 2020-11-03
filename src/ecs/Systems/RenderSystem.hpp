//https://github.com/awwdev

#pragma once
#include "com/Matrix.hpp"
#include "ecs/ComponentsMeta/ComponentEnum.hpp"
#include "ecs/ComponentsMeta/ComponentArrays.hpp"
#include "gpu/RenderData/RenderData.hpp"
#include "gpu/RenderData/RenderData_General.hpp"

namespace rpg::ecs {
    
inline void RenderSystem(ComponentArrays<>& arrays, const double, gpu::RenderData& renderData)
{
    /*
    FOR_ARRAY(arrays.mainComponents.dense, i) 
    {
        auto const& mainComponent = arrays.mainComponents.dense[i];
        if (mainComponent.meshEnum == res::MeshEnum::None)
            continue;

        //render data
        auto const meshEnumIdx = (idx_t) mainComponent.meshEnum;
        gpu::RenderData_General::InstanceData const instanceData {
        };


        //renderData.general.meshInstances[meshEnumIdx].AppendElement(meshInstance);   
    }
    */

}

}//ns