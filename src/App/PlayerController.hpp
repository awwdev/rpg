//https://github.com/awwdev

#pragma once
#include "ECS/ECS.hpp"
#include "Utils/Matrix.hpp"
#include "window/WindowEvents.hpp"
#include "Rendering/Cameras.hpp"

namespace rpg::app {
    
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
        auto& transform = ecs.arrays.AddComponent<ecs::ComponentType::Transform>(playerID);
        transform.scale = { S, H, S };
        transform.translation = { 0, Ypos, 0 };
        ecs.arrays.AddComponent<ecs::ComponentType::RenderData>(playerID, res::MeshType::PrimitiveCube);
    }

    void Update(const double dt, ecs::ECS& ecs)
    {
        if (wnd::global::resizeState == wnd::global::ResizeState::End)
            camera.UpdatePerspective();
            
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

        auto& playerTransform = ecs.arrays.transforms.Get(playerID);
        playerTransform.translation = { position[X], Ypos, position[Z] };
        playerTransform.rotation    = { 0, camera.rotation[Y], 0 };
        
        const utils::Vec3f pos = { position[X], HEAD, position[Z] };
        camera.Update(orientation, pos, dt);
    }
};
    
}//ns