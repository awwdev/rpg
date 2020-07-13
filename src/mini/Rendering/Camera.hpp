//https://github.com/awwdev

#pragma once
#include "mini/Window/AppEvents.hpp"
#include "mini/Math/Matrix.hpp"

namespace mini::rendering
{
    struct Camera
    {
        float x = 0;
        float y = 0;
        float z = 0;
        float spd = 2;

        void Update(const double dt)
        {
            if(wnd::asciiPressed == 'd'){
                x -= (float)dt * spd;
            }
            if(wnd::asciiPressed == 'a'){
                x += (float)dt * spd;
            }
            if(wnd::asciiPressed == 'w'){
                z += (float)dt * spd;
            }
            if(wnd::asciiPressed == 's'){
                z -= (float)dt * spd;
            }
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
             const math::Mat4f view {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                x, y, z, 1,
            };

            return view * projection;
        }
    };

}//ns