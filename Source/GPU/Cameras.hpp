//https://github.com/awwdev

#pragma once
#include "ECS/ECS.hpp"
#include "Common/Matrix.hpp"
#include "window/WindowEvents.hpp"

namespace rpg::gpu
{
struct EgoCamera
{
    com::Vec3f position { 0,  4, -4 };
    com::Vec3f rotation {45,  0,  0 };

    com::Mat4f perspective;
    com::Mat4f view;

    float mouseSpeed = 0.3f;
    float fov        = 45;
    float scrollSpd  = 0.1f;
    float moveSpeed  = 10;
    float acc        = 10;

    EgoCamera()
    {
        UpdatePerspective();
    }

    void Update(const double dt)
    {
        using namespace com;

        com::Vec3f movNorm {};
        if(wnd::HasEvent<wnd::EventType::D, wnd::EventState::PressedOrHeld>()) { movNorm[X] -= 1; }
        if(wnd::HasEvent<wnd::EventType::A, wnd::EventState::PressedOrHeld>()) { movNorm[X] += 1; }
        if(wnd::HasEvent<wnd::EventType::W, wnd::EventState::PressedOrHeld>()) { movNorm[Z] += 1; }
        if(wnd::HasEvent<wnd::EventType::S, wnd::EventState::PressedOrHeld>()) { movNorm[Z] -= 1; }
        NormalizeThis(movNorm);

        rotation[Y] += wnd::glo::mouse_dx * mouseSpeed; //!need of dt ?
        rotation[X] += wnd::glo::mouse_dy * mouseSpeed;

        if (rotation[X] >=  360) rotation[X] -= 360;
        if (rotation[Y] >=  360) rotation[Y] -= 360;
        if (rotation[X] <= -360) rotation[X] += 360;
        if (rotation[Y] <= -360) rotation[Y] += 360;

        const auto qX = QuatAngleAxis(+rotation[X], com::Vec3f{1, 0, 0});
        const auto qY = QuatAngleAxis(-rotation[Y], com::Vec3f{0, 1, 0});
        const auto qRot = com::QuatMultQuat(qY, qX);

        const auto movDir  = com::QuatMultVec(qRot, movNorm);
        const auto moveAcc = wnd::HasEvent<wnd::EventType::Shift, wnd::EventState::PressedOrHeld>() ? acc : 1.f;
        position = position + (movDir * moveSpeed * moveAcc * (float)dt);

        if (wnd::HasEvent<wnd::EventType::Mouse_Scroll>()) {
            fov -= wnd::glo::mouse_scroll_delta * scrollSpd;
            UpdatePerspective();
        }
        

        view = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            position[X], position[Y], position[Z], 1,
        };

        const auto mRot = QuatToMat(qRot);
        view = mRot * view;
    }

    void UpdatePerspective()
    {
        const float aspect = (float)wnd::glo::window_w / (float)wnd::glo::window_h;
        const float fovRad = fov * (3.14f / 180.f);
        const float n = 0.01f;
        const float f = 0; //infinity
        const float h = 1.f / tanf(fovRad * 0.5f);
        const float w = h / aspect;

        perspective = {
            w, 0, 0, 0,
            0, h, 0, 0,
            0, 0, f,-1,
            0, 0, n, 0,
        };
    }
};

struct ThirdCamera
{
    com::Vec3f rotation;
    float distance = 7;

    com::Mat4f perspective;
    com::Mat4f view;
    com::Mat4f mRot;
    com::Quatf qRot;

    float mouseSpeed = 0.1f;
    float fov        = 45;
    float scrollSpd  = 0.01f;

    ThirdCamera()
    {
        UpdatePerspective();
    }

    void Update(const com::Vec3f orientation, const com::Vec3f position, const double)
    {
        using namespace com;

        rotation[Y] += wnd::glo::mouse_dx * mouseSpeed; //!need of dt ?
        rotation[X] += wnd::glo::mouse_dy * mouseSpeed;

        if (rotation[X] >=  360) rotation[X] -= 360;
        if (rotation[Y] >=  360) rotation[Y] -= 360;
        if (rotation[X] <= -360) rotation[X] += 360;
        if (rotation[Y] <= -360) rotation[Y] += 360;

        const auto qX = QuatAngleAxis(+rotation[X], com::Vec3f{1, 0, 0});
        const auto qY = QuatAngleAxis(-rotation[Y], com::Vec3f{0, 1, 0});
        qRot = com::QuatMultQuat(qY, qX);
        mRot = QuatToMat(qRot);

        if (wnd::HasEvent<wnd::EventType::Mouse_Scroll>()) {
            distance -= wnd::glo::mouse_scroll_delta * scrollSpd;
        }
        

        com::Vec3f orientVec = (orientation * distance);
        com::Mat4f mOrientation = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            orientVec[X], orientVec[Y], orientVec[Z], 1,
        };

        view = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            -position[X], -position[Y], -position[Z], 1,
        };

        view = (mOrientation * mRot) * view;
    }

    void UpdatePerspective()
    {
        const float aspect = (float)wnd::glo::window_w / (float)wnd::glo::window_h;
        const float fovRad = fov * (3.14f / 180.f);
        const float n = 0.01f;
        const float f = 0; //infinity
        const float h = 1.f / tanf(fovRad * 0.5f);
        const float w = h / aspect;

        perspective = {
            w, 0, 0, 0,
            0, h, 0, 0,
            0, 0, f,-1,
            0, 0, n, 0,
        };
    }
};

template<class CAMERA>
com::Vec3f ScreenRay(const CAMERA& camera)
{
    using namespace com;

    const auto mx = (f32)wnd::glo::mouse_wx;
    const auto my = (f32)wnd::glo::mouse_wy;
    const auto ww = (f32)wnd::glo::window_w;
    const auto wh = (f32)wnd::glo::window_h;

    const com::Vec4f homo {
        ((mx / ww) * 2) - 1,
        ((my / wh) * 2) - 1,
        -1,                
        1
    };

    const auto projInv = Inverse(camera.perspective);
    auto eye = projInv * homo;
    eye[Z] = -1;
    eye[W] =  0;
    
    const auto viewInv = Inverse(camera.view);
    auto world = viewInv * eye;

    return { world[X], world[Y], world[Z] };
}

}//ns