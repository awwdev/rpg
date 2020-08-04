//https://github.com/awwdev

#pragma once
#include "mini/ECS/ECS.hpp"
#include "mini/Math/Matrix.hpp"
#include "mini/window/WindowEvents.hpp"
#include "mini/Rendering/Cameras.hpp"

namespace mini::app {
    
struct PlayerController
{
    rendering::ThirdCamera camera;

    ecs::ID playerID = 0;
    float speed = 5.f;
    math::Vec3f orientation { 0, 0, -1 };
    math::Vec3f position {};
    math::Vec3f rotation {};


    void Create(ecs::ECS& ecs)
    {
        playerID = ecs.AddEntity();
        ecs.arrays.AddComponent<ecs::ComponentType::Transform>(playerID, math::Identity4());
        ecs.arrays.AddComponent<ecs::ComponentType::RenderData>(playerID, res::MeshType::PrimitiveCube);
    }

    void Update(const double dt, ecs::ECS& ecs)
    {
        if (app::global::inputMode != app::global::PlayMode) return;

        using namespace math;

        Vec3f moveNorm {};
        if (wnd::HasEvent<wnd::W, wnd::PressedOrHeld>()) moveNorm[Z] -= 1;
        if (wnd::HasEvent<wnd::A, wnd::PressedOrHeld>()) moveNorm[X] -= 1;
        if (wnd::HasEvent<wnd::S, wnd::PressedOrHeld>()) moveNorm[Z] += 1;
        if (wnd::HasEvent<wnd::D, wnd::PressedOrHeld>()) moveNorm[X] += 1;
        NormalizeThis(moveNorm);
        const auto movDir = math::QuatMultVec(camera.qRot, moveNorm);
        auto move = movDir * speed * (float)dt;

        position = position + move;
        math::Mat4f mTransform = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            position[X], 0, position[Z], 1,
        };
        const auto qY = QuatAngleAxis(camera.rotation[Y], math::Vec3f{0, 1, 0});
        const auto mRot = QuatToMat(qY);
        const auto mOrient = mTransform * mRot;

        auto& playerTransform = ecs.arrays.transforms.Get(playerID);
        playerTransform.transform = mOrient;

        
        const math::Vec3f pos = { position[X], 0, position[Z] };
        camera.Update(orientation, pos, dt);
    }
};
    
}//ns