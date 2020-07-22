//https://github.com/awwdev

#pragma once
#include "mini/Window/AppEvents.hpp"
#include "mini/Math/Matrix.hpp"
#undef far
#undef near

namespace mini::rendering
{
    //TODO: ray point into scene

    struct Camera
    {
        math::Vec3f pos { 0, 0, 0 };
        math::Vec3f rotTarget  {}; 
        math::Vec3f movNorm {};
        math::Quatf qRot {};

        float movSpd    = 2;
        float dirSpd    = 150;
        float mouseSpd  = 0.001f;
        float fov       = 45;
        float scrollSpd = 0.01f;

        void Update(const double dt)
        {
            movNorm = {};
            if(wnd::asciiPressed['D']){
                movNorm[Vx] -= 1;
            }
            if(wnd::asciiPressed['A']){
                movNorm[Vx] += 1;
            }
            if(wnd::asciiPressed['W']){
                movNorm[Vz] += 1;
            }
            if(wnd::asciiPressed['S']){
                movNorm[Vz] -= 1;
            }
            NormalizeThis(movNorm);

            if (wnd::ui_mode == false) {
                rotTarget[Vy] += wnd::mouse_dx * mouseSpd;
                rotTarget[Vx] += wnd::mouse_dy * mouseSpd;
            }

            const auto qX = math::QuatAngleAxis(+rotTarget[Vx] * dirSpd, math::Vec3f{1, 0, 0});
            const auto qY = math::QuatAngleAxis(-rotTarget[Vy] * dirSpd, math::Vec3f{0, 1, 0});
            qRot = qY * qX;
            math::NormalizeThis(qRot);

            const auto movDir = movNorm * qRot;

            if (wnd::ui_mode == false) {
                pos += movDir * movSpd * (float)dt;
            }

            //? scroll

            if (const auto* ev = wnd::CheckEvent(wnd::EventType::Mouse_Scroll)){
                fov -= ev->scrollDelta * scrollSpd;
            }
        }

        math::Mat4f GetProjView() const
        {
            return GetProjection() * GetView();
        }

        math::Mat4f GetView() const 
        {
            const math::Mat4f mPos {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                pos[Vx], pos[Vy], pos[Vz], 1,
            };
            const auto mRot = math::ToMat4(qRot);
            return mRot * mPos;
        }

        math::Mat4f GetProjection() const 
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

        math::Vec3f ScreenRay() const
        {
            const auto mx = (f32)wnd::mouse_client_x;
            const auto my = (f32)wnd::mouse_client_y;
            const auto ww = (f32)wnd::window_w;
            const auto wh = (f32)wnd::window_h;

            const math::Vec4f homo {
                ((mx / ww) * 2) - 1,
                ((my / wh) * 2) - 1,
                -1,                
                1
            };

            const auto projInv = math::Inverse(GetProjection());
            auto eye = homo * projInv;
            eye[Vz] = -1;
            eye[Vw] =  0;
            
            const auto viewInv = math::Inverse(GetView());
            auto world = eye * viewInv;

            return { world[Vx], world[Vy], world[Vz] };
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