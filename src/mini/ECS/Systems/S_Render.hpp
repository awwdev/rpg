//https://github.com/awwdev

#pragma once
#include "mini/Math/Matrix.hpp"
#include "mini/ECS/Components.hpp"
#include "mini/ECS/ComponentArray.hpp"
#include "mini/Rendering/RenderGraph.hpp"

namespace mini::ecs
{
    //test placing a cube for matrix inversion
    inline void PlaceCubeTest(rendering::RenderGraph& renderGraph, math::Mat4f& transform)
    {
        //renderGraph.camera.ScreenRay();

        //normalisation from viewport to NDC
        math::Vec3f NDC  { 0.1f, 0.1f, 0 }; //center
        math::Vec4f homo { NDC[Vx], NDC[Vy], -1, 1 };
        
        const auto projInv = math::Inverse(renderGraph.camera.GetProjection());
        auto eye = homo * projInv;
        eye[Vz] = -1;
        eye[Vw] = 1;

        const auto viewInv = math::Inverse(renderGraph.camera.GetView());
        auto world = eye * viewInv;
        
        transform[3][0] = world[Vx] * 2;
        transform[3][1] = world[Vy] * 2;
    }

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

                //!hacked into (for testing)
                if (i == (u32)res::MeshType::PrimitiveCube)
                    PlaceCubeTest(renderGraph, transform.transform);

                auto rotMat = math::RotationMatrixY(rot);
                group.Append(transform.transform);// * rotMat);
            }

            if (!group.Empty()) 
                renderGraph.default_ubo.AppendGroup(group, (res::MeshType)i); //will only append if group is not empty
        }

    }

}//ns