#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Box/String.hpp"
#include "mini/Scene/IRenderer.hpp"

using namespace mini;

namespace mini::scenes
{
    struct Scene
    {
        box::String<10> name { "Scene1" };

        inline void Update(IRenderer& renderer, const double dt)
        {
            renderer.DrawBox(0, 0, 200, 50);

            box::String<100> fpsStr;
            fpsStr.Set("fps: ");
            char buf[20];
            sprintf_s(buf, "%4.0f", 1/dt);
            fpsStr.Append(buf);

            renderer.DrawText(fpsStr.dataPtr, 0, 0);

        }

    };

}//ns