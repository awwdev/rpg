//https://github.com/awwdev

#pragma once
#include "mini/Window/AppEvents.hpp"
#include "mini/Math/Matrix.hpp"

namespace mini::rendering
{
    struct Camera
    {
        math::Vec3f pos { 0, 0, -5};
        math::Vec3f rot {};
        float spd = 2;

        void Update(const double dt)
        {
            if(wnd::asciiPressed == 'd'){
                pos[Vx] -= (float)dt * spd;
            }
            if(wnd::asciiPressed == 'a'){
                pos[Vx] += (float)dt * spd;
            }
            if(wnd::asciiPressed == 'w'){
                pos[Vz] += (float)dt * spd;
            }
            if(wnd::asciiPressed == 's'){
                pos[Vz] -= (float)dt * spd;
            }

            rot[Vy] += wnd::mouse_dx * 0.001f;
            rot[Vx] += wnd::mouse_dy * 0.001f;
        }

        math::Mat4f GetMat() const
        {
            const float aspect = (float)wnd::window_w / (float)wnd::window_h;
            const float fov = 0.785f; //rad
            const float n = 0.01f;
            const float f = 0; //infinity
            const float h = 1.f / std::tanf(fov * 0.5f);
            const float w = h / aspect;

            const math::Mat4f projection {
                w, 0, 0, 0,
                0, h, 0, 0,
                0, 0, f,-1,
                0, 0, n, 0,
            };
            const math::Mat4f posMat {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                pos[Vx], pos[Vy], pos[Vz], 1,
            };
            const auto rotViewY = math::RotationMatrixY(-rot[Vy]);
            const auto rotViewX = math::RotationMatrixX(+rot[Vx]);

            static float r = 0;
            r += 0.01f;
            auto quat = math::QuatAngleAxis(r, math::Vec3f{0, 1, 0});
            auto quatMat = math::ToMat4(quat);
            
            //return rotViewY * rotViewX * posMat * projection;
            return quatMat * posMat * projection;
        }
    };

}//ns