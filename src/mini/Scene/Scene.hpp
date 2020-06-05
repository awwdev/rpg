#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Box/String.hpp"

using namespace mini;

namespace mini::scenes
{
    struct Scene
    {
        box::String<10> name { "Scene1" };

        inline void Update(const double dt)
        {
            //LOG("update ", name);
        }

    };

}//ns