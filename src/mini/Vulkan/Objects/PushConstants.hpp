//https://github.com/awwdev

#pragma once

#include "mini/Utils/Types.hpp"
#include "mini/Math/Matrix.hpp"
#include "mini/Vulkan/Context.hpp"

namespace mini::vk
{
    struct Common_PushConstants
    {
        math::Mat4f camera;
        math::Mat4f sun;
    };

    struct UI_PushConstants
    {
        u32 wnd_w;
        u32 wnd_h;
    };

}//ns