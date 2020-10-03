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

        const auto& s = mainComponent.scale;
        const auto& r = mainComponent.rotation;
        const auto& t = mainComponent.translation;

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
        auto combinedMat = scaleMat * rotMat;
        combinedMat[3][0] = t.x;
        combinedMat[3][1] = t.y;
        combinedMat[3][2] = t.z;

        //render data
        auto const meshEnumIdx = (idx_t) mainComponent.meshEnum;
        gpu::RenderData_General::MeshInstance const meshInstance {
            .transform = combinedMat
        };
        renderData.general.meshInstances[meshEnumIdx].Append(meshInstance);        
    }

}

}//ns