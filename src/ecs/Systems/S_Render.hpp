//https://github.com/awwdev

#pragma once
#include "com/Matrix.hpp"
#include "ecs/Components/Meta/ComponentEnum.hpp"
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

        const auto& renderComponent    = renderComponents.dense[i];
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
        auto combinedMat = scaleMat * rotMat;
        combinedMat[3][0] = t.x;
        combinedMat[3][1] = t.y;
        combinedMat[3][2] = t.z;

        //render data
        const auto modelType = (idx_t) renderComponent.modelType;

        gpu::RenderData_General::MeshInstance modelInstance{};
        modelInstance.transform = combinedMat;

        //! render needs to sort based on meshes
        //! prefabs are based on modeltype but would need to create entities per submesh
        //! meshIdx would be a combination of model type + submesh idx
        //! need of parenting

        //renderData.general.meshStats[modelType].instanceCount += 1;
        //renderData.general.meshInstances.Append(modelInstance);
    }
}

}//ns