//https://github.com/awwdev

#pragma once
#include "com/Structs.hpp"
#include "com/Algorithms.hpp"
#include "ecs/ECS.hpp"
#include "gpu/RenderData/RenderData.hpp"

namespace rpg::gpu {

struct Sun
{
    com::Vec3f pos { 0, -1, 0 }; //xz controlled by sin
    float t = 0;
    ecs::ID gizmoID = 0;
    float speed = 0.001f;

    void Create(ecs::ECS&)
    {
        //gizmoID = ecs.AddEntity();
        //ecs.arrays.AddComponent<ecs::ComponentType::Transform> (gizmoID, use::Identity4());
        //ecs.arrays.AddComponent<ecs::ComponentType::RenderData>(gizmoID, res::MeshType::PrimitiveCube);
    }

    void Update(ecs::ECS&, const double dt, RenderData& renderData)
    {
        
        using namespace com;
        t += (float)dt * speed;
        constexpr auto A = 2;
        pos.x = sinf(t) * A;
        pos.z = cosf(t) * A;

        //auto& cubeTrans = ecs.arrays.transforms.Get(gizmoID);
        //cubeTrans.transform = {
        //    1, 0, 0, 0,
        //    0, 1, 0, 0,
        //    0, 0, 1, 0,
        //    pos.x, pos.y, pos.z, 1,
        //};

        constexpr com::Mat4f biasMat {
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.5, 0.5, 0.0, 1.0 
        };

        for(uint32_t i = 0; i < RenderData_Shadow::CASCADE_COUNT; ++i){
            renderData.shadow.uboShadowMap.projView[i]       = GetOrthographic(i) * GetView();
            renderData.shadow.uboShadowMap.projViewBiased[i] = biasMat * GetOrthographic(i) * GetView();
        }
        
    }

    com::Mat4f GetView() const
    {
        return com::LookAt(pos, {0, 0, 0});
    }

    com::Mat4f GetOrthographic(const u32 cascadeIdx) const 
    {
        //TODO: solve how the values correlate
        //TODO: cascades
        float S {};
        switch(cascadeIdx)
        {
            case 0: S = 0.005f; break;
            case 1: S = 0.050f; break;
            case 2: S = 0.100f; break;
            case 3: S = 0.200f; break;
        }
        //const float W = 0.001f + ((f32)cascadeIdx * 0.1f);//1 / 1024.f;//1 / vuk::g_contextPtr->surfaceCapabilities.currentExtent.width;
        //const float H = 0.001f + ((f32)cascadeIdx * 0.1f);//1 / 1024.f;//1 / vuk::g_contextPtr->surfaceCapabilities.currentExtent.height;
        const float D = 0.00001f; 
        const float Z = 0.01f;

        return {
            S, 0, 0, 0,
            0, S, 0, 0,
            0, 0, D, 0,
            0, 0, Z, 1,
        };
    }
};

}//ns