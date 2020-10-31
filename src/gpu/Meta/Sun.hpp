//https://github.com/awwdev

#pragma once

#include "com/Matrix.hpp"
#include "gpu/RenderData/RenderData.hpp"
#include "gpu/RenderData/RenderData_Shadow.hpp"
#include "gpu/Meta/Camera.hpp"

namespace rpg::gpu {

struct Sun 
{
    using RD = RenderData_Shadow;

    com::Mat4f view;
    com::Mat4f projections [RD::CASCADE_COUNT];

    com::Vec3f position { 0, 0, 0 };;
    com::Vec3f rotation { 45, 0, 0 };
    com::Vec3f direction {};

    void Create()
    {
        //TODO: gizmo

        //scale / zoom
        constexpr float SCALES [RenderData_Shadow::CASCADE_COUNT]
        {
            0.1000f, //close
            0.0500f,
            0.0250f,
            0.0125f, //far
        };

        constexpr float D = 0.000001f; //far
        constexpr float Z = 0.01f;     //near

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

    void Update(Camera const& camera)
    {
        //const auto p = com::Vec3f { 45, 0, 45 }; //(position + camera.position + com::Vec3f { camera.direction.x, 0, camera.direction.z } * 20);
        const auto p = camera.position; //(position + camera.position + com::Vec3f { camera.direction.x, 0, camera.direction.z } * 20);
        view = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            -p.x, -p.y, -p.z, 1,
        };

        const auto qX = QuatAngleAxis(+rotation.x, com::Vec3f { 1, 0, 0 });
        const auto qY = QuatAngleAxis(-rotation.y, com::Vec3f { 0, 1, 0 });
        const auto qRot = com::QuatMultQuat(qY, qX);
        const auto mRot = QuatToMat(qRot);
        const auto dir = mRot * com::Vec4f{ 0, 0, 1, 1 };
        direction = { dir.x, dir.y, -dir.z };

        view = mRot * view;
    }

    void UpdateRenderData(RenderData& renderData) const
    {
        constexpr com::Mat4f BIAS_MAT {
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.5, 0.5, 0.0, 1.0 
        };

        renderData.shadow.uboShadowMap.sunDir = direction;
        for(uint32_t i = 0; i < RD::CASCADE_COUNT; ++i){
            renderData.shadow.uboShadowMap.projView[i] = projections[i] * view;
            renderData.shadow.uboShadowMap.projViewBiased[i] = BIAS_MAT * projections[i] * view;
        }
    }

};

}//ns