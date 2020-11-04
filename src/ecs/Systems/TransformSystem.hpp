//https://github.com/awwdev

#pragma once
#include "com/Matrix.hpp"
#include "ecs/ComponentsMeta/ComponentArrays.hpp"

namespace rpg::ecs {
    
inline void TransformSystem(ComponentArrays<>& arrays, const double)
{
    FOR_ARRAY(arrays.transform_components.dense, i) 
    {
        auto& transform_component = arrays.transform_components.dense[i];
        transform_component.transform = com::Identity4();
        //this way it is possible to combine nested transforms (children)
    }

    FOR_ARRAY(arrays.transform_components.dense, i) 
    {
        auto& transform_component = arrays.transform_components.dense[i];

        const auto& s = transform_component.scale;
        const auto& r = transform_component.rotation;
        const auto& t = transform_component.translation;

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

        transform_component.transform = transform_component.transform * combinedMat;

        FOR_ARRAY(transform_component.children, i) {
            auto const childID = transform_component.children[i];
            auto& child_transform_component = arrays.transform_components.Get(childID);
            child_transform_component.transform = child_transform_component.transform * combinedMat;
        }
    }
}

}//ns