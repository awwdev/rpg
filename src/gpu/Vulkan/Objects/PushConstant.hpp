//https://github.com/awwdev

#pragma once
#include "gpu/RenderData/_Old/RenderStructs.hpp"
#include "gpu/Vulkan/Meta/Context.hpp"

namespace rpg::gpu::vuk {

template<class T>
struct PushConstant
{
    T data;
    VkPushConstantRange rangeInfo;
};

}//ns