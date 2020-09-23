//https://github.com/awwdev

#pragma once
#include "com/Matrix.hpp"
#include "ecs/Components/Meta/ComponentTypes.hpp"
#include "ecs/Components/Meta/ComponentArrays.hpp"
#include "gpu/RenderData/RenderData.hpp"
#include "gpu/RenderData/RenderData_General.hpp"

namespace rpg::ecs {
    
inline void S_Render(ComponentArrays<>& arrays, const double, gpu::RenderData& renderData)
{
    const auto& renderComponents    = arrays.renderComponents;
    const auto& transformComponents = arrays.transforms;

    renderData.general.sboModels.Append(gpu::RenderData_General::SBO_Model{
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    });
    renderData.general.sboModels.Append(gpu::RenderData_General::SBO_Model{
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        1, 0, 0, 1,
    });

    FOR_ARRAY(renderComponents.dense, i) 
    {
        const auto& renderComponent = renderComponents.dense[i];
        const auto& meshType = renderComponent.meshType;

    }


    /*
    com::Array<ecs::ID, ecs::MAX_ENTITY_COUNT> meshTypesVertexColor [(u32)res::MeshType::ENUM_END];
    com::Array<ecs::ID, ecs::MAX_ENTITY_COUNT> meshTypesTextures    [(u32)res::MeshType::ENUM_END];

    FOR_ARRAY(arr_render.dense, i) {
        const auto idx = (idx_t)arr_render.dense[i].meshType;
        if (arr_render.dense[i].useTexture) 
            meshTypesTextures[idx].Append(arr_render.entityLookup[i]);
        else
            meshTypesVertexColor[idx].Append(arr_render.entityLookup[i]);
    }   

    auto loop = [&](bool useTexture, auto& arr) {
        com::Array<gpu::Common_UniformData, gpu::DEFAULT_UBO_MAX_COUNT> group;

        FOR_CARRAY(arr, i){ // meshType == group
            group.Clear();
            FOR_ARRAY(arr[i], j){
                const auto eID  = arr[i][j];
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

                //TODO: REMOVE
                if (arr_render.Get(eID).meshType == res::MeshType::Tree){
                    group.Append(tMat);//, 1);
                    continue;
                }

                group.Append(tMat);//, 0);
            }

            if (!group.Empty()) {
                if (useTexture)
                    renderGraph.common_ubo.AppendGroup_Texture(group, (res::MeshType)i);
                else
                    renderGraph.common_ubo.AppendGroup_VertexColor(group, (res::MeshType)i);
            } 
        }

    };
    
    loop(false, meshTypesVertexColor);
    loop(true, meshTypesTextures);
    */
}

}//ns