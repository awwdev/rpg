#pragma once
#include "mini/Debug/Logger.hpp"
#include "mini/Box/String.hpp"

namespace app::scene
{
    struct Scene
    {
        mini::box::String<10> name { "Scene1" };


        inline void Update(const double dt)
        {
            //LOG("update ", name);
        }

    };

}//ns