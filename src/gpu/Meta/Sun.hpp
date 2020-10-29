//https://github.com/awwdev

#pragma once

#include "com/Matrix.hpp"
#include "gpu/RenderData/RenderData.hpp"
#include "gpu/RenderData/RenderData_Shadow.hpp"

namespace rpg::gpu {

struct Sun 
{
    using RD = RenderData_Shadow;

    com::Mat4f view;
    com::Mat4f projections [RD::CASCADE_COUNT];

    com::Vec3f position { 0, -1, 0 };;
    com::Vec3f rotation { 45, 0, 0 };

    void Create()
    {
        //TODO: gizmo

        //scale / zoom
        constexpr float SCALES [RenderData_Shadow::CASCADE_COUNT]
        {
            0.005f,
            0.050f,
            0.100f,
            0.200f,
        };

        constexpr float D = 0.00001f; //far
        constexpr float Z = 0.01f;    //near

        for(uint32_t i = 0; i < RD::CASCADE_COUNT; ++i)
        {
            const auto& S = SCALES[i];
            projections [i] = {
                S, 0, 0, 0,
                0, S, 0, 0,
                0, 0, D, 0,
                0, 0, Z, 1,
            };
        }
    }

    void Update( com::Vec3f const& cameraPos)
    {
        //TODO: follow camera frustrum
        const auto& p = position * -1;
        view = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            p.x, p.y, p.z, 1,
        };

        const auto qX = QuatAngleAxis(+rotation.x, com::Vec3f{1, 0, 0});
        const auto qY = QuatAngleAxis(-rotation.y, com::Vec3f{0, 1, 0});
        const auto qRot = com::QuatMultQuat(qY, qX);
        const auto mRot = QuatToMat(qRot);

        view = mRot * view;

        //TODO: not exactly the camera pos, but somewhere at the beginning of the frustrum
    }

    void UpdateRenderData(RenderData& renderData) const
    {
        constexpr com::Mat4f BIAS_MAT {
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.5, 0.5, 0.0, 1.0 
        };

        renderData.shadow.uboShadowMap.sunDir = Normalize(position);
        for(uint32_t i = 0; i < RD::CASCADE_COUNT; ++i){
            renderData.shadow.uboShadowMap.projView[i] = projections[i] * view;
            renderData.shadow.uboShadowMap.projViewBiased[i] = BIAS_MAT * projections[i] * view;
        }
    }

};

}//ns