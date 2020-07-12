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

        static float rot = 0; //test
        rot += 1.f * (float)dt;

        box::Array<rendering::UniformData_Default, 500> uboCubes;
        FOR_ARRAY(arr_render.dense, i) {
            const auto eID  = arr_render.entityLookup[i];
            auto& transform = arr_transform.Get(eID);
            auto rotMat     = math::RotationMatrixY(rot);
            uboCubes.Append(rotMat * transform.transform);
        }

        renderGraph.default_uboGroups.Append(rendering::UniformGroup{ 
            .begin = renderGraph.default_uboArray.count, 
            .count = uboCubes.Count()
        });
        renderGraph.default_uboArray.AppendArray(uboCubes);
    }

}//ns

/*
rendering::UniformData_Default cubes [500];
for(auto y = 0; y < 10; ++y)
for(auto x = 0; x < 50; ++x)
{
auto rotMat1 = math::RotationMatrixX(0.5f+y);
auto rotMat2 = math::RotationMatrixY(rot);
rotMat2 *= rotMat1;
rotMat2[3][2] = -100.f;
rotMat2[3][0] = x * 5.f;
rotMat2[3][1] = y * 5.f;
cubes [y * 50 + x] = {
    rotMat2
};
}*/