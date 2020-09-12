//https://github.com/awwdev

#pragma once

#include "Common/Types.hpp"
#include "Common/Matrix.hpp"
#include "Vulkan/Context.hpp"

namespace rpg::vk {

struct Common_PushConstants
{
    use::Mat4f camera;
    use::Mat4f sun;
    use::Mat4f sunBias;
    use::Vec3f sunDir;
    float time;
};

struct Common_PushConstants2
{
    use::Mat4f projection;
    use::Mat4f view;
    use::Mat4f sunView;
    use::Vec3f sunDir;
};

struct Shadow_PushConstants
{
    use::Mat4f sunCasc [3];
    uint32_t cascade;
};

struct Sky_PushConstants
{
    use::Mat4f camera;
    use::Mat4f sun;
    use::Vec4f topColor;
    use::Vec4f botColor;
};

struct UI_PushConstants
{
    u32 wnd_w;
    u32 wnd_h;
};

}//ns