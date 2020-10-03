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
    FOR_ARRAY(arrays.mainComponents.dense, i) 
    {
        auto const& mainComponent = arrays.mainComponents.dense[i];

        //render data
        auto const meshEnumIdx = (idx_t) mainComponent.meshEnum;
        gpu::RenderData_General::MeshInstance const meshInstance {
            .transform = mainComponent.transform
        };
        renderData.general.meshInstances[meshEnumIdx].Append(meshInstance);        
    }

}

}//ns