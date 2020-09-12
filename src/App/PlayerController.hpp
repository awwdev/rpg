//https://github.com/awwdev

#pragma once
#include "ECS/ECS.hpp"
#include "Common/Matrix.hpp"
#include "window/WindowEvents.hpp"
#include "GPU/Cameras.hpp"

namespace rpg::app {
    
struct PlayerController
{
    gpu::ThirdCamera camera;

    ecs::ID playerID = 0;
    float speed = 5.f;
    com::Vec3f orientation { 0, 0, -1 };
    com::Vec3f position {};
    com::Vec3f rotation {};

    static constexpr float S = 0.2f;
    static constexpr float H = 0.5f;
    static constexpr float Ypos = -0.5f;
    static constexpr float HEAD = -1.0f;

    void Create(ecs::ECS& ecs)
    {
        using namespace com;
        playerID = ecs.AddEntity();
        auto& transform = ecs.arrays.AddComponent<ecs::ComponentType::Transform>(playerID);
        transform.scale = { S, H, S };
        transform.translation = { 0, Ypos, 0 };
        ecs.arrays.AddComponent<ecs::ComponentType::RenderData>(playerID, res::MeshType::PrimitiveCube);
    }

    void Update(const double dt, ecs::ECS& ecs)
    {
        if (wnd::glo::resizeState == wnd::glo::ResizeState::End)
            camera.UpdatePerspective();
            
        if (app::glo::inputMode != app::glo::PlayMode) return;

        using namespace com;

        Vec3f moveNorm {};
        if (wnd::HasEvent<wnd::EventType::W, wnd::EventState::PressedOrHeld>()) moveNorm[Z] -= 1;
        if (wnd::HasEvent<wnd::EventType::A, wnd::EventState::PressedOrHeld>()) moveNorm[X] -= 1;
        if (wnd::HasEvent<wnd::EventType::S, wnd::EventState::PressedOrHeld>()) moveNorm[Z] += 1;
        if (wnd::HasEvent<wnd::EventType::D, wnd::EventState::PressedOrHeld>()) moveNorm[X] += 1;
        NormalizeThis(moveNorm);
        const auto movDir = com::QuatMultVec(camera.qRot, moveNorm);
        auto move = movDir * speed * (float)dt;

        position = position + move;

        auto& playerTransform = ecs.arrays.transforms.Get(playerID);
        playerTransform.translation = { position[X], Ypos, position[Z] };
        playerTransform.rotation    = { 0, camera.rotation[Y], 0 };
        
        const com::Vec3f pos = { position[X], HEAD, position[Z] };
        camera.Update(orientation, pos, dt);
    }
};
    
}//ns