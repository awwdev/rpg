//https://github.com/awwdev

#pragma once
#include "GPU/RenderStructs.hpp"
#include "GPU/Vulkan/Meta/Context.hpp"

namespace rpg::gpu::vuk {

template<class T>
struct PushConstant
{
    T data;
    VkPushConstantRange rangeInfo;
};

}//ns