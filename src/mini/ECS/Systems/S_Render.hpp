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

        //SORTING
        box::Array<ecs::ID, 100> meshTypes [resources::MeshType::ENUM_END];
        FOR_ARRAY(arr_render.dense, i) {
            const auto idx = arr_render.dense[i].meshType;
            meshTypes[idx].Append(arr_render.entityLookup[i]);
        }   

        box::Array<rendering::UniformData_Default, 100> ubos;
        FOR_CARRAY(meshTypes, i){
            if (meshTypes[i].Empty()) continue;
            ubos.Clear();
            FOR_ARRAY(meshTypes[i], j){
                const auto eID  = meshTypes[i][j];
                auto& transform = arr_transform.Get(eID);
                auto rotMat     = math::RotationMatrixY(rot);//!test
                ubos.Append(rotMat * transform.transform);
            }

            renderGraph.default_uboGroups.Append(rendering::UniformGroup{
                .begin = renderGraph.default_uboArray.count, 
                .count = ubos.Count()
            });
            renderGraph.default_uboArray.AppendArray(ubos);
        }

    }

}//ns