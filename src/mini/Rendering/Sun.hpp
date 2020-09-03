//https://github.com/awwdev

#pragma once
#include "mini/Utils/Structs.hpp"
#include "mini/Utils/Algorithms.hpp"
#include "mini/ECS/ECS.hpp"

namespace mini::rendering {

struct Sun
{
    utils::Vec3f pos { 0, -1, 0 }; //xz controlled by sin
    float t = 0;
    ecs::ID gizmoID = 0;
    float speed = 0.001f;

    void Create(ecs::ECS& ecs)
    {
        //gizmoID = ecs.AddEntity();
        //ecs.arrays.AddComponent<ecs::ComponentType::Transform> (gizmoID, utils::Identity4());
        //ecs.arrays.AddComponent<ecs::ComponentType::RenderData>(gizmoID, res::MeshType::PrimitiveCube);
    }

    void Update(ecs::ECS& ecs, const double dt)
    {
        
        using namespace utils;
        t += (float)dt * speed;
        constexpr auto A = 2;
        pos[X] = std::sinf(t) * A;
        pos[Z] = std::cosf(t) * A;

        //auto& cubeTrans = ecs.arrays.transforms.Get(gizmoID);
        //cubeTrans.transform = {
        //    1, 0, 0, 0,
        //    0, 1, 0, 0,
        //    0, 0, 1, 0,
        //    pos[X], pos[Y], pos[Z], 1,
        //};
        
    }

    utils::Mat4f GetView() const
    {
        return utils::LookAt(pos, {0, 0, 0});
    }

    utils::Mat4f GetOrthographic() const 
    {
        //TODO: solve how the values correlate
        //TODO: cascades
        const float W = 0.02f;//1 / 1024.f;//1 / vk::g_contextPtr->surfaceCapabilities.currentExtent.width;
        const float H = 0.02f;//1 / 1024.f;//1 / vk::g_contextPtr->surfaceCapabilities.currentExtent.height;
        const float D = 0.00001f; 
        const float Z = 0.01f;

        return {
            W, 0, 0, 0,
            0, H, 0, 0,
            0, 0, D, 0,
            0, 0, Z, 1,
        };
    }
};

}//ns