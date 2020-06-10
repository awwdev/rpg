#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Box/String.hpp"
#include "mini/App/IRenderer.hpp"
#include "mini/App/UI.hpp"

using namespace mini;

namespace mini::app
{
    struct Scene
    {
        box::String<10> name { "Scene1" };

        inline void Update(IRenderer& renderer, const double dt)
        {
            if (ui::Button(renderer, "Button", {200, 200, 120, 30}) == ui::ButtonState::Released) {
                LOG("button pressed");
            }
     

            box::String<100> fpsStr;
            fpsStr.Set("fps: ");
            char buf[20];
            sprintf_s(buf, "%4.0f", 1/dt);
            fpsStr.Append(buf);

            static int counter = 0;
            counter += 1;
            const auto x = std::sin(counter * 0.001f) * 100; //just to move text a bit

            /*renderer.Add_DrawText(R"(Lorem ipsum dolor sit amet, 
consetetur sadipscing elitr,
sed diam nonumy eirmod tempor invidunt,
ut labore et dolore magna aliquyam erat,
sed diam voluptua. At vero eos et accusam et,
justo duo dolores et ea rebum.Stet clita kasd,
no sea takimata sanctus est Lorem ipsum dolor,
Lorem ipsum dolor sit amet, consetetur,
sed diam nonumy eirmod tempor invidunt ut labore,
sed diam voluptua. At vero eos et accusam et justo duo,
)"
            , x, 0);*/

        }

    };

}//ns