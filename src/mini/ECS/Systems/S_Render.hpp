//https://github.com/awwdev

#pragma once
#include "mini/Utils/Matrix.hpp"
#include "mini/ECS/Components/Meta/ComponentTypes.hpp"
#include "mini/ECS/Components/Meta/ComponentArrays.hpp"
#include "mini/Rendering/RenderGraph.hpp"

namespace mini::ecs {
    
inline void S_Render(ComponentArrays<>& arrays, const double dt, rendering::RenderGraph& renderGraph)
{
    using namespace utils;

    auto& arr_render    = arrays.renderData;
    auto& arr_transform = arrays.transforms;

    //? SORTING (for mesh type)(so instancing can be used)
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
            const auto& s = arr_transform.Get(eID).scale;
            const auto& r = arr_transform.Get(eID).rotation;
            const auto& t = arr_transform.Get(eID).translation;

            //? compose matrix
            const Mat4f sMat = {
                s[X], 0, 0, 0,
                0, s[Y], 0, 0,
                0, 0, s[Z], 0,
                0, 0, 0, 1,
            };

            const auto qX = QuatAngleAxis(r[X], { 1, 0, 0 });
            const auto qY = QuatAngleAxis(r[Y], { 0, 1, 0 });
            const auto qZ = QuatAngleAxis(r[Z], { 0, 0, 1 });
            auto q1 = QuatMultQuat(qX, qY);
            auto q2 = QuatMultQuat(q1, qZ);
            NormalizeThis(q2);
            const auto qMat = QuatToMat(q2);

            auto tMat = sMat * qMat;
            tMat[3][X] = t[X];
            tMat[3][Y] = t[Y];
            tMat[3][Z] = t[Z];

            group.Append(tMat);
        }

        if (!group.Empty()) 
            renderGraph.default_ubo.AppendGroup(group, (res::MeshType)i); //will only append if group is not empty
    }

}

}//ns