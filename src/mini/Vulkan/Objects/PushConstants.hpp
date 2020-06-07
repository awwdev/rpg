//https://github.com/awwdev

#pragma once

#include "mini/Utils/Types.hpp"
#include "mini/Vulkan/Context.hpp"

namespace mini::vk
{
    //offset must be less than VkPhysicalDeviceLimits::maxPushConstantsSize
    //!min requirement is 128 anyways
    //offset must be a multiple of 4
    //size must be greater than 0
    //size must be a multiple of 4
    //size must be less than or equal to VkPhysicalDeviceLimits::maxPushConstantsSize minus offset

    struct Default_PushConstants
    {
        u32 wnd_w, wnd_h;
    };

}//ns