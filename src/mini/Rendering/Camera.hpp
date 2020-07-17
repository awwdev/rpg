//https://github.com/awwdev

#pragma once
#include "mini/Window/AppEvents.hpp"
#include "mini/Math/Matrix.hpp"

namespace mini::rendering
{
    struct Camera
    {
        math::Vec3f pos { 0, 0, 0 }; //!should be minus
        math::Vec3f rot {};
        math::Vec3f normMov {};
        math::Quatf qRot {};

        float movSpd = 2;
        float dirSpd = 150;

        void Update(const double dt)
        {
            normMov = {};
            if(wnd::asciiPressed == 'd'){
                normMov[Vx] -= 1;
            }
            if(wnd::asciiPressed == 'a'){
                normMov[Vx] += 1;
            }
            if(wnd::asciiPressed == 'w'){
                normMov[Vz] += 1;
            }
            if(wnd::asciiPressed == 's'){
                normMov[Vz] -= 1;
            }
            NormalizeThis(normMov);

            rot[Vy] += wnd::mouse_dx * 0.001f;
            rot[Vx] += wnd::mouse_dy * 0.001f;

            const auto qX = math::QuatAngleAxis(+rot[Vx] * dirSpd, math::Vec3f{1, 0, 0});
            const auto qY = math::QuatAngleAxis(-rot[Vy] * dirSpd, math::Vec3f{0, 1, 0});
            qRot = qY * qX;
            math::NormalizeThis(qRot);

            const auto mov = normMov * qRot;
            pos += mov * movSpd * (float)dt;
        }

        math::Mat4f GetMat() const
        {
            const float aspect = (float)wnd::window_w / (float)wnd::window_h;
            const float fov = 0.785f; //rad
            const float n = 0.01f;
            const float f = 0.0001f; //infinity
            const float h = 1.f / std::tanf(fov * 0.5f);
            const float w = h / aspect;

            const math::Mat4f projection {
                w, 0, 0, 0,
                0, h, 0, 0,
                0, 0, f,-1,
                0, 0, n, 0,
            };

            //TODO: just multiply last column
            const math::Mat4f mPos {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                pos[Vx], pos[Vy], pos[Vz], 1,
            };

            const auto mRot = math::ToMat4(qRot);
            return mPos * mRot * projection;
        }
    };

}//ns