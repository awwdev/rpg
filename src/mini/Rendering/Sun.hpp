//https://github.com/awwdev

#pragma once
#include "mini/Utils/Structs.hpp"
#include "mini/Utils/Algorithms.hpp"
#include "mini/ECS/ECS.hpp"

namespace mini::rendering
{
    struct Sun
    {
        math::Vec3f pos { -1, -40, -1 }; //xz controlled by sin
        float t = 0;
        ecs::ID gizmoID = 0;

        void Create(ecs::ECS& ecs)
        {
            gizmoID = ecs.AddEntity();
            ecs.arrays.AddComponent<ecs::ComponentType::Transform> (gizmoID, math::Identity4());
            ecs.arrays.AddComponent<ecs::ComponentType::RenderData>(gizmoID, res::MeshType::PrimitiveCube);
        }

        void Update(ecs::ECS& ecs, const double dt)
        {
            using namespace math;
            t += (float)dt * 0.5f;
            constexpr auto A = 100;
            pos[X] = std::sinf(t) * A;
            pos[Z] = std::cosf(t) * A;

            auto& cubeTrans = ecs.arrays.transforms.Get(gizmoID);
            cubeTrans.transform = {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                pos[X], pos[Y], pos[Z], 1,
            };
        }

        math::Mat4f GetView() const
        {
            return math::LookAt(pos, {0, 0, 0});
        }
    };

}//ns