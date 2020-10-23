//https://github.com/awwdev

#pragma once
#include "wnd/WindowEvents.hpp"
#include "com/Matrix.hpp"
#include "gpu/RenderData/RenderData.hpp"

namespace rpg::gpu {

struct Camera
{
    //? projection
    enum class ProjectionMode { Perspective, Orthographic } projectionMode;
    com::Mat4f projection;
    com::Mat4f view; 
    f32 fov;
    f32 far;
    f32 near;

    //? transform
    com::Vec3f position;
    com::Vec3f rotation;

    //? input
    enum class PersonMode { FirstPerson, ThirdPerson } personMode;
    f32 mouseSpeed  = 0.1f;
    f32 scrollSpeed = 0.01f;
    f32 moveSpeed = 10;
    f32 moveSpeedMultiplier = 10; //when shift is pressed

    //? rays
    com::Ray cameraRay; //center of window
    com::Ray mouseRay;

    //? methods

    void UpdateView(float const dt)
    {
        //rotation (in degrees)
        rotation.y += wnd::glo::mouse_dx * mouseSpeed;
        rotation.x += wnd::glo::mouse_dy * mouseSpeed;

        //rotation clamping
        if (rotation.x >=  360) rotation.x -= 360;
        if (rotation.y >=  360) rotation.y -= 360;
        if (rotation.x <= -360) rotation.x += 360;
        if (rotation.y <= -360) rotation.y += 360;

        //rotation matrix and quaternions
        const auto qX = QuatAngleAxis(+rotation.x, com::Vec3f{1, 0, 0});
        const auto qY = QuatAngleAxis(-rotation.y, com::Vec3f{0, 1, 0});
        const auto qRot = com::QuatMultQuat(qY, qX);
        const auto mRot = QuatToMat(qRot);

        //move input
        com::Vec3f moveInput {};
        if(wnd::HasEvent<wnd::EventType::D, wnd::EventState::PressedOrHeld>()) { moveInput.x -= 1; }
        if(wnd::HasEvent<wnd::EventType::A, wnd::EventState::PressedOrHeld>()) { moveInput.x += 1; }
        if(wnd::HasEvent<wnd::EventType::W, wnd::EventState::PressedOrHeld>()) { moveInput.z += 1; }
        if(wnd::HasEvent<wnd::EventType::S, wnd::EventState::PressedOrHeld>()) { moveInput.z -= 1; }
        NormalizeThis(moveInput);

        //position update based on rotation
        const auto movDir  = com::QuatMultVec(qRot, moveInput);
        const auto shiftPressed = wnd::HasEvent<wnd::EventType::Shift, wnd::EventState::PressedOrHeld>();
        const auto moveAcc = shiftPressed ? moveSpeedMultiplier : 1.f;
        position = position + (movDir * moveSpeed * moveAcc * dt);

        //? view (translation and rotation)
        view = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            position.x, position.y, position.z, 1,
        };
        view = mRot * view;

        //scrolling
        if (wnd::HasEvent<wnd::EventType::Mouse_Scroll>()) {
            fov -= wnd::glo::mouse_scroll_delta * scrollSpeed;
            UpdateProjection();
        }
    }

    void UpdateProjection()
    {
        if (projectionMode == ProjectionMode::Perspective)
        {
            auto const fovRad = fov * (3.14f / 180.f);
            auto const n = near;
            auto const f = far; 
            auto const h = 1.f / tanf(fovRad * 0.5f);
            auto const w = h / wnd::glo::window_ratio;
            projection = {
                w, 0, 0, 0,
                0, h, 0, 0,
                0, 0, f,-1,
                0, 0, n, 0,
            };
        }
    }

    void UpdateRenderData(gpu::RenderData& renderData) const
    {
        renderData.general.meta.view = view;
        renderData.general.meta.proj = projection;
        renderData.general.meta.viewDir = com::Normalize(rotation);
    }
};

}//ns