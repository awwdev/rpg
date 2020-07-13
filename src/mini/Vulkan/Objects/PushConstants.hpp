//https://github.com/awwdev

#pragma once

#include "mini/Utils/Types.hpp"
#include "mini/Math/Matrix.hpp"
#include "mini/Vulkan/Context.hpp"

namespace mini::vk
{
    struct Common_PushConstants
    {
        math::Mat4f projection;
        //camera pos
        u32 wnd_w, wnd_h;
    };

}//ns