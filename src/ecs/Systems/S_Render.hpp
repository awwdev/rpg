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

    FOR_ARRAY(renderComponents.dense, i) 
    {
        const auto entityID = renderComponents.entityLookup[i];

        const auto& renderComponent = renderComponents.dense[i];
        const auto& transformComponent = transformComponents.Get(entityID);

        const auto& s = transformComponent.scale;
        const auto& r = transformComponent.rotation;
        const auto& t = transformComponent.translation;

        //scale
        const com::Mat4f scaleMat = {
            s.x,   0,   0,   0,
              0, s.y,   0,   0,
              0,   0, s.z,   0,
              0,   0,   0,   1,
        };

        //rotation
        const auto qX = com::QuatAngleAxis(r.x, { 1, 0, 0 });
        const auto qY = com::QuatAngleAxis(r.y, { 0, 1, 0 });
        const auto qZ = com::QuatAngleAxis(r.z, { 0, 0, 1 });
        auto q1 = QuatMultQuat(qX, qY);
        auto q2 = QuatMultQuat(q1, qZ);
        NormalizeThis(q2);
        const auto rotMat = QuatToMat(q2);

        //translation
        auto transMat = scaleMat * rotMat;
        transMat[3][0] = t.x;
        transMat[3][1] = t.y;
        transMat[3][2] = t.z;

        //render data
        gpu::RenderData_General::SBO_Model sboModel {};
        sboModel.transform = transMat;
        renderData.general.sboModels.Append(sboModel);

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
                    s.x, 0, 0, 0,
                    0, s.y, 0, 0,
                    0, 0, s.z, 0,
                    0, 0, 0, 1,
                };

                const auto qX = QuatAngleAxis(r.x, { 1, 0, 0 });
                const auto qY = QuatAngleAxis(r.y, { 0, 1, 0 });
                const auto qZ = QuatAngleAxis(r.z, { 0, 0, 1 });
                auto q1 = QuatMultQuat(qX, qY);
                auto q2 = QuatMultQuat(q1, qZ);
                NormalizeThis(q2);
                const auto qMat = QuatToMat(q2);

                auto tMat = sMat * qMat;
                tMat[3].x = t.x;
                tMat[3].y = t.y;
                tMat[3].z = t.z;

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