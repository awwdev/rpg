//https://github.com/awwdev

#pragma once

#include "Common/Types.hpp"
#include "Common/Matrix.hpp"
#include "Vulkan/Context.hpp"

namespace rpg::vk {

struct Common_PushConstants
{
    com::Mat4f camera;
    com::Mat4f sun;
    com::Mat4f sunBias;
    com::Vec3f sunDir;
    float time;
};

struct Common_PushConstants2
{
    com::Mat4f projection;
    com::Mat4f view;
    com::Mat4f sunView;
    com::Vec3f sunDir;
};

struct Shadow_PushConstants
{
    com::Mat4f sunCasc [3];
    uint32_t cascade;
};

struct Sky_PushConstants
{
    com::Mat4f camera;
    com::Mat4f sun;
    com::Vec4f topColor;
    com::Vec4f botColor;
};

struct UI_PushConstants
{
    u32 wnd_w;
    u32 wnd_h;
};

}//ns