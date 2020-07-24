//https://github.com/awwdev

#pragma once
#include "mini/Utils/PrimitiveMeshes.hpp"
#include "mini/Debug/Assert.hpp"
#include "mini/Resources/Mesh.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/ECS/ECS.hpp"
#include "mini/Rendering/Camera.hpp"

namespace mini::res
{
    struct Terrain
    {
        static constexpr auto QUAD_COUNT_Z = 10;
        static constexpr auto QUAD_COUNT_X = 10;

        box::SimpleArray<utils::Common_Vertex, QUAD_COUNT_Z * QUAD_COUNT_X * 6> verts {};

        ecs::ID gizmoID = 0;


        void Create(ecs::ECS& ecs)
        {
            verts = res::CreateMeshGrid<QUAD_COUNT_X, QUAD_COUNT_Z>(50, 50);

            //GIZMO CUBE
            {
                constexpr float S =  0.05f;
                constexpr float X =  0; 
                constexpr float Y =  0;
                constexpr float Z =  0;
                gizmoID = ecs.AddEntity();
                const math::Mat4f pos = math::Mat4f{
                    S, 0, 0, 0,
                    0, S, 0, 0,
                    0, 0, S, 0,
                    X, Y, Z, 1,
                };
                ecs.arrays.AddComponent<ecs::ComponentType::Transform> (gizmoID, pos);
                ecs.arrays.AddComponent<ecs::ComponentType::RenderData>(gizmoID, res::MeshType::PrimitiveCube);
            }
        }

        void Update(const double dt, const rendering::Camera& camera, ecs::ECS& ecs)
        {
            //INTERSECTION
            using namespace utils;
            const auto ray = camera.ScreenRay();
            for(auto i = 0; i < verts.CAPACITY; i+=3)
            {
                const auto& v0 = verts[i+0].pos;
                const auto& v1 = verts[i+1].pos;
                const auto& v2 = verts[i+2].pos;
                const auto intersection = utils::RayTriangleIntersection(
                    camera.pos,
                    ray,
                    { v0[Vx], v0[Vy], v0[Vz] },
                    { v1[Vx], v1[Vy], v1[Vz] },
                    { v2[Vx], v2[Vy], v2[Vz] }
                );
                if (intersection) {
                    auto X = intersection->pos[Vx];
                    auto Y = intersection->pos[Vy];
                    auto Z = intersection->pos[Vz];

                    X = v0[Vx];
                    Y = v0[Vy];
                    Z = v0[Vz];

                    if (intersection->u > 0.5){
                        X = v1[Vx];
                        Y = v1[Vy];
                        Z = v1[Vz];
                    }
                    else if (intersection->v > 0.5) {
                        X = v2[Vx];
                        Y = v2[Vy];
                        Z = v2[Vz];
                    }
                    else if (intersection->u > 0.25 && intersection->v > 0.25)
                    {
                        if (intersection->u > intersection->v){
                            X = v1[Vx];
                            Y = v1[Vy];
                            Z = v1[Vz];
                        }
                        else {
                            X = v2[Vx];
                            Y = v2[Vy];
                            Z = v2[Vz];
                        }
                    }

                    constexpr float S = 1;
                    auto& cubeTrans = ecs.arrays.transforms.Get(gizmoID);
                    cubeTrans.transform = {
                        S, 0, 0, 0,
                        0, S, 0, 0,
                        0, 0, S, 0,
                        X, Y, Z, 1,
                    };
                }
            }
        }

    };
    
}//ns