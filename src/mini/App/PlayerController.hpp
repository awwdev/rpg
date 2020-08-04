//https://github.com/awwdev

#pragma once
#include "mini/ECS/ECS.hpp"
#include "mini/Math/Matrix.hpp"
#include "mini/window/WindowEvents.hpp"
#include "mini/Rendering/Cameras.hpp"

namespace mini::app
{
struct PlayerController
{
    rendering::ThirdCamera camera;

    ecs::ID playerID = 0;
    float speed = 5.f;
    math::Vec3f orientation { 0, -1, -1 };

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
        if (wnd::HasEvent<wnd::W, wnd::PressedOrHeld>()) moveNorm[Z] += 1;
        if (wnd::HasEvent<wnd::A, wnd::PressedOrHeld>()) moveNorm[X] += 1;
        if (wnd::HasEvent<wnd::S, wnd::PressedOrHeld>()) moveNorm[Z] -= 1;
        if (wnd::HasEvent<wnd::D, wnd::PressedOrHeld>()) moveNorm[X] -= 1;
        NormalizeThis(moveNorm);
        auto move = moveNorm * speed * (float)dt;

        auto& playerTransform = ecs.arrays.transforms.Get(playerID);
        playerTransform.transform[3][0] += move[X];
        playerTransform.transform[3][1] += move[Y];
        playerTransform.transform[3][2] += move[Z];

        camera.Update(dt);
    }
};
    
}//ns