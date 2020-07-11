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
            const float n = 0.01f;
            const float f = 100.f;
            const float fov = 45.f;
            const float aspect = (float)wnd::window_w / (float)wnd::window_h;
            const float tanFov = std::tanf(fov * 0.5f);
            const float w = 1.f / (tanFov * aspect);
            const float h = 1.f / (tanFov);
            const float c = -(f+n) / (f-n);
            const float d = -2*(f*n) / (f-n);

            const math::Mat4f projection {
                w, 0, 0, 0,
                0, h, 0, 0,
                0, 0, c,-1,
                0, 0, d, 1,
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