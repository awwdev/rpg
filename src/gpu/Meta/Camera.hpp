//https://github.com/awwdev

#pragma once
#include "wnd/WindowEvents.hpp"
#include "com/Matrix.hpp"
#include "gpu/RenderData/RenderData.hpp"

namespace rpg::gpu {

struct Camera
{
    //? projection
    enum class ProjectionMode { Perspective, Orthographic } projectionMode = ProjectionMode::Perspective;
    com::Mat4f projection;
    com::Mat4f view; 
    f32 fov  = 45;
    f32 far  = 0;
    f32 near = 0.01f;

    //? transform
    com::Vec3f position { 0, 0, 0 };
    com::Vec3f rotation { 0, 0, 0 }; //euler in degree
    com::Vec3f direction {};

    //? input
    enum class PersonMode { FirstPerson, ThirdPerson } personMode;
    f32 mouseSpeed = 0.3f;
    f32 scrollSpeed = 0.05f;
    f32 moveSpeed = 10;
    f32 moveSpeedMultiplier = 10; //when shift is pressed
    com::Vec3f targetPosition {};
    f32 targetDistance {};

    //? rays
    com::Ray cameraRay {}; //center of window
    com::Ray mouseRay  {};

    //? methods

    Camera()
    {
        UpdateView(0);
        UpdateProjection();
    }

    void UpdateView(double const dt)
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
        const auto qX = QuatAngleAxis(+rotation.x, com::Vec3f { 1, 0, 0 });
        const auto qY = QuatAngleAxis(-rotation.y, com::Vec3f { 0, 1, 0 });
        const auto qRot = com::QuatMultQuat(qY, qX);
        const auto mRot = QuatToMat(qRot);
        const auto dir = mRot * com::Vec4f{ 0, 0, 1, 1 }; //TODO: try again with -1 for z
        direction = { dir.x, dir.y, -dir.z };
        

        //move input
        com::Vec3f moveInput {};
        if(wnd::HasEvent<wnd::EventType::D, wnd::EventState::PressedOrHeld>()) { moveInput.x += 1; }
        if(wnd::HasEvent<wnd::EventType::A, wnd::EventState::PressedOrHeld>()) { moveInput.x -= 1; }
        if(wnd::HasEvent<wnd::EventType::W, wnd::EventState::PressedOrHeld>()) { moveInput.z -= 1; }
        if(wnd::HasEvent<wnd::EventType::S, wnd::EventState::PressedOrHeld>()) { moveInput.z += 1; }
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
            -position.x, -position.y, -position.z, 1,
        };
        view = mRot * view;
        //scrolling
        if (wnd::HasEvent<wnd::EventType::Mouse_Scroll>()) {
            fov -= wnd::glo::mouse_scroll_delta * scrollSpeed;
            UpdateProjection();
        }
    }

    void UpdateRays() 
    {
        mouseRay.origin = position;
        mouseRay.direction = ScreenRay(wnd::glo::mouse_window_x, wnd::glo::mouse_window_y) * 1;
        cameraRay.origin = position;
        cameraRay.direction = direction;
    }

    void UpdateProjection()
    {
        auto const fovRad = fov * (3.14f / 180.f);
        auto const n = near;
        auto const f = far; 
        auto const h = 1.f / tanf(fovRad * 0.5f);
        auto const w = h / wnd::glo::window_aspect_ratio;

        if (projectionMode == ProjectionMode::Perspective)
        {
            projection = {
                w, 0, 0, 0,
                0, h, 0, 0,
                0, 0, f,-1,
                0, 0, n, 0,
            };
        }
        if (projectionMode == ProjectionMode::Orthographic)
        {
            projection = {
                w, 0, 0, 0,
                0, h, 0, 0,
                0, 0, f, 0,
                0, 0, n, 1,
            };
        }
    }

    void UpdateRenderData(gpu::RenderData& renderData) const
    {
        renderData.general.meta.view = view;
        renderData.general.meta.proj = projection;
        renderData.general.meta.viewDir = direction;
        renderData.general.meta.viewPos = position;
    }

    com::Vec3f ScreenRay(f32 const x, f32 const y) const
    {
        const auto ww = (f32)wnd::glo::window_w;
        const auto wh = (f32)wnd::glo::window_h;

         const com::Vec4f homo {
            ((x / ww) * 2) - 1,
            ((y / wh) * 2) - 1,
            -1,                
            1
        };

        const auto projInv = Inverse(projection);
        auto eye = projInv * homo;
        eye.z = -1;
        eye.w =  0;
        
        const auto viewInv = Inverse(view);
        auto world = viewInv * eye;

        return { world.x, world.y, world.z };
    }

};

}//ns