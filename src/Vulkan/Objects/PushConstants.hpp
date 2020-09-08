//https://github.com/awwdev

#pragma once

#include "Utils/Types.hpp"
#include "Utils/Matrix.hpp"
#include "Vulkan/Context.hpp"

namespace rpg::vk {

struct Common_PushConstants
{
    utils::Mat4f camera;
    utils::Mat4f sun;
    utils::Mat4f sunBias;
    utils::Vec3f sunDir;
    float time;
};

struct Common_PushConstants2
{
    utils::Mat4f projection;
    utils::Mat4f view;
    utils::Mat4f sunView;
    utils::Vec3f sunDir;
};

struct Shadow_PushConstants
{
    utils::Mat4f sunCasc [3];
    uint32_t cascade;
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