//https://github.com/awwdev

#pragma once
#include "mini/Math/Matrix.hpp"
#include "mini/ECS/Components.hpp"
#include "mini/ECS/ComponentArray.hpp"
#include "mini/Rendering/RenderGraph.hpp"

namespace mini::ecs
{
    inline void S_Render(ComponentArrays<>& arrays, const double dt, rendering::RenderGraph& renderGraph)
    {
        auto& arr_render    = arrays.renderData;
        auto& arr_transform = arrays.transforms;

        static float rot = 0; //!test
        rot += 1.f * (float)dt;

        //SORTING (for mesh type)
        box::Array<ecs::ID, ecs::MAX_ENTITY_COUNT> meshTypes [(u32)res::MeshType::ENUM_END];
        FOR_ARRAY(arr_render.dense, i) {
            const auto idx = (u32)arr_render.dense[i].meshType;
            meshTypes[idx].Append(arr_render.entityLookup[i]);
        }   

        box::Array<rendering::Default_UniformData, rendering::DEFAULT_UBO_MAX_COUNT> group;
        FOR_CARRAY(meshTypes, i){ // meshType == group

            group.Clear();
            FOR_ARRAY(meshTypes[i], j){
                const auto eID  = meshTypes[i][j];
                auto& transform = arr_transform.Get(eID);
                auto rotMat = math::MatAngleAxisY(rot);
                group.Append(transform.transform);// * rotMat);
            }

            if (!group.Empty()) 
                renderGraph.default_ubo.AppendGroup(group, (res::MeshType)i); //will only append if group is not empty
        }

    }

}//ns