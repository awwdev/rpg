//https://github.com/awwdev

#pragma once
#include "mini/ECS/ECS.hpp"
#include "mini/Utils/Matrix.hpp"
#include "mini/window/WindowEvents.hpp"
#include "mini/Rendering/Cameras.hpp"

namespace mini::app {
    
struct PlayerController
{
    rendering::ThirdCamera camera;

    ecs::ID playerID = 0;
    float speed = 5.f;
    utils::Vec3f orientation { 0, 0, -1 };
    utils::Vec3f position {};
    utils::Vec3f rotation {};

    static constexpr float S = 0.2f;
    static constexpr float H = 0.5f;
    static constexpr float Ypos = -0.5f;
    static constexpr float HEAD = -1.0f;

    void Create(ecs::ECS& ecs)
    {
        using namespace utils;
        playerID = ecs.AddEntity();
        ecs.arrays.AddComponent<ecs::ComponentType::Transform>(playerID, utils::Mat4f{
            S, 0, 0, 0,
            0, H, 0, 0,
            0, 0, S, 0,
            position[X], Ypos, position[Z], 1,
        });
        ecs.arrays.AddComponent<ecs::ComponentType::RenderData>(playerID, res::MeshType::PrimitiveCube);
    }

    void Update(const double dt, ecs::ECS& ecs)
    {
        if (app::global::inputMode != app::global::PlayMode) return;

        using namespace utils;

        Vec3f moveNorm {};
        if (wnd::HasEvent<wnd::W, wnd::PressedOrHeld>()) moveNorm[Z] -= 1;
        if (wnd::HasEvent<wnd::A, wnd::PressedOrHeld>()) moveNorm[X] -= 1;
        if (wnd::HasEvent<wnd::S, wnd::PressedOrHeld>()) moveNorm[Z] += 1;
        if (wnd::HasEvent<wnd::D, wnd::PressedOrHeld>()) moveNorm[X] += 1;
        NormalizeThis(moveNorm);
        const auto movDir = utils::QuatMultVec(camera.qRot, moveNorm);
        auto move = movDir * speed * (float)dt;

        position = position + move;
        utils::Mat4f mTransform = {
            S, 0, 0, 0,
            0, H, 0, 0,
            0, 0, S, 0,
            position[X], Ypos, position[Z], 1,
        };
        const auto qY = QuatAngleAxis(camera.rotation[Y], utils::Vec3f{0, 1, 0});
        const auto mRot = QuatToMat(qY);
        const auto mOrient = mTransform * mRot;

        auto& playerTransform = ecs.arrays.transforms.Get(playerID);
        playerTransform.transform = mOrient;
        
        const utils::Vec3f pos = { position[X], HEAD, position[Z] };
        camera.Update(orientation, pos, dt);
    }
};
    
}//ns