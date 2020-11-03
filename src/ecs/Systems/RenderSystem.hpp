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
    FOR_ARRAY(arrays.render_components.dense, i) 
    {
        auto const& entityID = arrays.render_components.entityLookup[i];
        auto const& render_component = arrays.render_components.dense[i];
        auto const& transform_component = arrays.transform_components.Get(entityID);
        
        auto const meshEnum = (idx_t) render_component.mesh_enum;
        auto const matEnum  = (idx_t) render_component.material_enum;
        auto& instance_datas = renderData.general.instance_datas[matEnum][meshEnum];

        gpu::RenderData_General::InstanceData const instData {
            .transform = transform_component.transform,
            .render_settings = render_component.render_settings,
        };
        instance_datas.AppendElement();       
    }
}

}//ns