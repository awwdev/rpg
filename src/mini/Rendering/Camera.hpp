//https://github.com/awwdev

#pragma once
#include "mini/Window/AppEvents.hpp"
#include "mini/Math/Matrix.hpp"

namespace mini::rendering
{
    struct Camera
    {
        math::Vec3f pos { 0, 0, -5 };
        math::Vec3f rot {};
        math::Vec3f movNorm {};
        math::Quatf qRot {};

        float movSpd = 2;
        float dirSpd = 150;

        void Update(const double dt)
        {
            movNorm = {};
            if(wnd::asciiPressed == 'd'){
                movNorm[Vx] -= 1;
            }
            if(wnd::asciiPressed == 'a'){
                movNorm[Vx] += 1;
            }
            if(wnd::asciiPressed == 'w'){
                movNorm[Vz] += 1;
            }
            if(wnd::asciiPressed == 's'){
                movNorm[Vz] -= 1;
            }
            NormalizeThis(movNorm);

            rot[Vy] += wnd::mouse_dx * 0.001f;
            rot[Vx] += wnd::mouse_dy * 0.001f;

            const auto qX = math::QuatAngleAxis(+rot[Vx] * dirSpd, math::Vec3f{1, 0, 0});
            const auto qY = math::QuatAngleAxis(-rot[Vy] * dirSpd, math::Vec3f{0, 1, 0});
            qRot = qY * qX;
            math::NormalizeThis(qRot);

            const auto movDir = movNorm * qRot;
            pos += movDir * movSpd * (float)dt;
        }

        math::Mat4f GetMat() const
        {
            const float aspect = (float)wnd::window_w / (float)wnd::window_h;
            const float fov = 0.785f; //rad
            const float n = 0.01f;
            const float f = 0; //infinity
            const float h = 1.f / std::tanf(fov * 0.5f);
            const float w = h / aspect;

            const math::Mat4f mProj {
                w, 0, 0, 0,
                0, h, 0, 0,
                0, 0, f,-1,
                0, 0, n, 0,
            };
            
            //http://dev.theomader.com/depth-precision/
            //x (left -, right +)
            //y (up -, down +)
            //z (inwards -, toYou +) - z is reversed

            const math::Mat4f mPos {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                pos[Vx], pos[Vy], pos[Vz], 1,
            };
            const auto mRot = math::ToMat4(qRot);
            const auto mView = mRot * mPos;

            const auto mLook = math::LookAt(math::Vec3f{0, -1, -4}, math::Vec3f{0, 0, 0});

            return mProj * mView;
        }
    };

}//ns