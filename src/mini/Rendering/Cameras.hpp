//https://github.com/awwdev

#pragma once
#include "mini/ECS/ECS.hpp"
#include "mini/Utils/Matrix.hpp"
#include "mini/window/WindowEvents.hpp"

namespace mini::rendering
{
struct EgoCamera
{
    utils::Vec3f position { 0,  4, -4 };
    utils::Vec3f rotation {45,  0,  0 };

    utils::Mat4f perspective;
    utils::Mat4f view;

    float mouseSpeed = 0.3f;
    float fov        = 45;
    float scrollSpd  = 0.1f;
    float moveSpeed  = 10;

    EgoCamera()
    {
        UpdatePerspective();
    }

    void Update(const double dt)
    {
        using namespace utils;

        utils::Vec3f movNorm {};
        if(wnd::HasEvent<wnd::D, wnd::PressedOrHeld>()) { movNorm[X] -= 1; }
        if(wnd::HasEvent<wnd::A, wnd::PressedOrHeld>()) { movNorm[X] += 1; }
        if(wnd::HasEvent<wnd::W, wnd::PressedOrHeld>()) { movNorm[Z] += 1; }
        if(wnd::HasEvent<wnd::S, wnd::PressedOrHeld>()) { movNorm[Z] -= 1; }
        NormalizeThis(movNorm);

        rotation[Y] += wnd::global::mouse_dx * mouseSpeed; //!need of dt ?
        rotation[X] += wnd::global::mouse_dy * mouseSpeed;

        if (rotation[X] >=  360) rotation[X] -= 360;
        if (rotation[Y] >=  360) rotation[Y] -= 360;
        if (rotation[X] <= -360) rotation[X] += 360;
        if (rotation[Y] <= -360) rotation[Y] += 360;

        const auto qX = QuatAngleAxis(+rotation[X], utils::Vec3f{1, 0, 0});
        const auto qY = QuatAngleAxis(-rotation[Y], utils::Vec3f{0, 1, 0});
        const auto qRot = utils::QuatMultQuat(qY, qX);

        const auto movDir  = utils::QuatMultVec(qRot, movNorm);
        const auto moveAcc = wnd::HasEvent<wnd::Shift, wnd::PressedOrHeld>() ? 3.f : 1.f;
        position = position + (movDir * moveSpeed * moveAcc * (float)dt);

        if (wnd::HasEvent<wnd::Mouse_Scroll>()) {
            fov -= wnd::global::mouse_scroll_delta * scrollSpd;
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
        const float aspect = (float)wnd::global::window_w / (float)wnd::global::window_h;
        const float fovRad = fov * (3.14f / 180.f);
        const float n = 0.01f;
        const float f = 0; //infinity
        const float h = 1.f / std::tanf(fovRad * 0.5f);
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
    utils::Vec3f rotation;
    float distance = 7;

    utils::Mat4f perspective;
    utils::Mat4f view;
    utils::Mat4f mRot;
    utils::Quatf qRot;

    float mouseSpeed = 0.1f;
    float fov        = 45;
    float scrollSpd  = 0.01f;

    ThirdCamera()
    {
        UpdatePerspective();
    }

    void Update(const utils::Vec3f orientation, const utils::Vec3f position, const double dt)
    {
        using namespace utils;

        rotation[Y] += wnd::global::mouse_dx * mouseSpeed; //!need of dt ?
        rotation[X] += wnd::global::mouse_dy * mouseSpeed;

        if (rotation[X] >=  360) rotation[X] -= 360;
        if (rotation[Y] >=  360) rotation[Y] -= 360;
        if (rotation[X] <= -360) rotation[X] += 360;
        if (rotation[Y] <= -360) rotation[Y] += 360;

        const auto qX = QuatAngleAxis(+rotation[X], utils::Vec3f{1, 0, 0});
        const auto qY = QuatAngleAxis(-rotation[Y], utils::Vec3f{0, 1, 0});
        qRot = utils::QuatMultQuat(qY, qX);
        mRot = QuatToMat(qRot);

        if (wnd::HasEvent<wnd::Mouse_Scroll>()) {
            distance -= wnd::global::mouse_scroll_delta * scrollSpd;
        }
        

        utils::Vec3f orientVec = (orientation * distance);
        utils::Mat4f mOrientation = {
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
        const float aspect = (float)wnd::global::window_w / (float)wnd::global::window_h;
        const float fovRad = fov * (3.14f / 180.f);
        const float n = 0.01f;
        const float f = 0; //infinity
        const float h = 1.f / std::tanf(fovRad * 0.5f);
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
utils::Vec3f ScreenRay(const CAMERA& camera)
{
    using namespace utils;

    const auto mx = (f32)wnd::global::mouse_wx;
    const auto my = (f32)wnd::global::mouse_wy;
    const auto ww = (f32)wnd::global::window_w;
    const auto wh = (f32)wnd::global::window_h;

    const utils::Vec4f homo {
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