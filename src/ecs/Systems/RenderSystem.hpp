//https://github.com/awwdev

#pragma once
#include "com/Matrix.hpp"
#include "ecs/ComponentsMeta/ComponentEnum.hpp"
#include "ecs/ComponentsMeta/ComponentArrays.hpp"
#include "gpu/RenderData/RenderData.hpp"
#include "gpu/RenderData/RenderData_Main.hpp"

namespace rpg::ecs {
    
inline void RenderSystem(ComponentArrays<>& arrays, const double, gpu::RenderData& renderData)
{
    FOR_ARRAY(arrays.render_components.dense, i) 
    {
        auto const& entityID = arrays.render_components.entityLookup[i];
        auto const& render_component = arrays.render_components.dense[i];
        auto const& transform_component = arrays.transform_components.Get(entityID);
        
        if (render_component.meshEnum == res::MeshEnum::None)
            continue;

        auto const meshEnum = (idx_t) render_component.meshEnum;
        auto const matEnum  = (idx_t) render_component.materialEnum;
        auto& instance_datas = renderData.main.instance_datas[matEnum][meshEnum];

        gpu::RenderData_Main::InstanceData const instData {
            .transform = transform_component.transform,
            .materialSettings = render_component.materialSettings,
        };
        instance_datas.AppendElement(instData);       
    }   
}

}//ns