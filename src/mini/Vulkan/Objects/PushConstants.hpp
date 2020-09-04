//https://github.com/awwdev

#pragma once

#include "mini/Utils/Types.hpp"
#include "mini/Utils/Matrix.hpp"
#include "mini/Vulkan/Context.hpp"

namespace mini::vk
{
    struct Common_PushConstants
    {
        utils::Mat4f camera;
        utils::Mat4f sun;
        utils::Mat4f sunBias;
        utils::Vec3f sunDir;
        float time;
        int cascadeIndex;
    };

    struct Sky_PushConstants
    {
        utils::Mat4f camera;
        utils::Mat4f sun;
        utils::Vec4f topColor;
        utils::Vec4f botColor;
    };

    struct UI_PushConstants
    {
        u32 wnd_w;
        u32 wnd_h;
    };

}//ns