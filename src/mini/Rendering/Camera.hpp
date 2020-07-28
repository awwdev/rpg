//https://github.com/awwdev

#pragma once
#include "mini/Window/AppEvents.hpp"
#include "mini/Math/Matrix.hpp"
#include "mini/Box/Optional.hpp"
#undef far
#undef near

namespace mini::rendering
{
    struct Camera
    {
        math::Vec3f pos         { 0, 4, -4 };
        math::Vec3f rotTarget   { 0.5f, 0, 0 }; 
        math::Vec3f movNorm {};
        math::Quatf qRot {};

        float movSpd    = 10;
        float dirSpd    = 150;
        float mouseSpd  = 0.002f;
        float fov       = 45;
        float scrollSpd = 0.1f;

        void Update(const double dt)
        {
            using namespace math;

            movNorm = {};
            if(wnd2::IsPressed<wnd2::D>()) { movNorm[X] -= 1; }
            if(wnd2::IsPressed<wnd2::A>()) { movNorm[X] += 1; }
            if(wnd2::IsPressed<wnd2::W>()) { movNorm[Z] += 1; }
            if(wnd2::IsPressed<wnd2::S>()) { movNorm[Z] -= 1; }
            NormalizeThis(movNorm);

            if (wnd2::g_ui_mode == false) {
                rotTarget[Y] += wnd2::g_mouse_delta_x * mouseSpd;
                rotTarget[X] += wnd2::g_mouse_delta_y * mouseSpd;
            }

            const auto qX = QuatAngleAxis(+rotTarget[X] * dirSpd, math::Vec3f{1, 0, 0});
            const auto qY = QuatAngleAxis(-rotTarget[Y] * dirSpd, math::Vec3f{0, 1, 0});
            qRot = math::QuatMultQuat(qY, qX);
            NormalizeThis(qRot);

            const auto movDir = math::QuatMultVec(qRot, movNorm);

            if (wnd2::g_ui_mode == false) {
                pos = pos + (movDir * movSpd * (float)dt);
            }

            //? scroll
            if (wnd2::g_input[wnd2::Mouse_Scroll] == wnd2::Yes) {
                fov -= wnd2::g_mouse_scroll_delta * scrollSpd;
            }
        }

        math::Mat4f GetProjView() const
        {
            return GetPerspective() * GetView();
        }

        math::Mat4f GetView() const 
        {
            using namespace math;
            const Mat4f mPos {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                pos[X], pos[Y], pos[Z], 1,
            };
            const auto mRot = QuatToMat(qRot);
            return mRot * mPos;
        }

        math::Mat4f GetPerspective() const 
        {
            //reversed z
            const float aspect = (float)wnd::window_w / (float)wnd::window_h;
            const float fovRad = fov * (3.14f / 180.f);
            const float n = 0.01f;
            const float f = 0; //infinity
            const float h = 1.f / std::tanf(fovRad * 0.5f);
            const float w = h / aspect;

            return {
                w, 0, 0, 0,
                0, h, 0, 0,
                0, 0, f,-1,
                0, 0, n, 0,
            };
        }

        math::Mat4f GetOrthographic() const 
        {
            const float W = 0.1f;//1 / 1024.f;//1 / vk::g_contextPtr->surfaceCapabilities.currentExtent.width;
            const float H = 0.1f;//1 / 1024.f;//1 / vk::g_contextPtr->surfaceCapabilities.currentExtent.height;
            const float D = 0.0001f;
            const float Z = 0.01f;

            return {
                W, 0, 0, 0,
                0, H, 0, 0,
                0, 0, D, 0,
                0, 0, Z, 1,
            };
        }

        math::Vec3f ScreenRay() const
        {
            using namespace math;

            const auto mx = (f32)wnd2::g_mouse_window_x;
            const auto my = (f32)wnd2::g_mouse_window_y;
            const auto ww = (f32)wnd::window_w;
            const auto wh = (f32)wnd::window_h;

            const math::Vec4f homo {
                ((mx / ww) * 2) - 1,
                ((my / wh) * 2) - 1,
                -1,                
                1
            };

            const auto projInv = Inverse(GetPerspective());
            auto eye = projInv * homo;
            eye[Z] = -1;
            eye[W] =  0;
            
            const auto viewInv = Inverse(GetView());
            auto world = viewInv * eye;

            return { world[X], world[Y], world[Z] };
        }
    };

}//ns



/*
rotate around

static float t = 0;
t += 0.01f;
const float sx = std::sinf(t) * 2;
const float sz = std::cosf(t) * 2;

const auto mLook = math::LookAt({ sx, -2, sz}, {0,0,0});

*/

//http://dev.theomader.com/depth-precision/
//x (left -, right +)
//y (up -, down +)
//z (inwards -, toYou +) - z is reversed
//on NDC it gets mapped to z [0,1]

/*
not inversed

const auto mProj = [&]{

    const float aspect = (float)wnd::window_w / (float)wnd::window_h;
    const float fovRad = fov * (3.14f / 180.f);
    const float near   = 0.1f;
    const float far    = 100;
    const float f = -far / (far-near);
    const float n = -(far*near)/(far-near);
    const float h = 1.f / std::tanf(fovRad * 0.5f);
    const float w = h / aspect;

    return math::Mat4f {
        w, 0, 0, 0,
        0, h, 0, 0,
        0, 0, f,-1,
        0, 0, n, 0,
    };

}();

*/