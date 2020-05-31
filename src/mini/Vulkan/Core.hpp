//https://github.com/awwdev

#pragma once
#pragma warning(disable : 26812) //unscoped vk enum

#include "mini/Utils/Types.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Debug/Assert.hpp"
#include "mini/Box/SimpleArray.hpp"

#define VK_USE_PLATFORM_WIN32_KHR
#include "../third/include/vulkan.h"


//#ifdef DEBUG
#define VK_CHECK(fn) \
if (const auto res = fn; res != VK_SUCCESS) \
{ \
    if (res > 0) \
        WARN("VK RESULT", res); \
    else \
        mini::Assert(false, res); \
} 
//#else
//#   define VK_CHECK(fn) fn
//#endif // DEBUG

namespace mini::vk
{
    struct Refs
    {
        VkDevice device;
        Refs(VkDevice pDevice) : device { pDevice } {;}
    };
}//ns


/*
use this to give a resource a name to view in renderdoc

const VkDebugUtilsObjectNameInfoEXT nameInfo
{
    .sType          = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
    .pNext          = nullptr,
    .objectType     = VK_OBJECT_TYPE_IMAGE,
    .objectHandle   = (uint64_t)image_font.image,
    .pObjectName    = "font image"
};
((PFN_vkSetDebugUtilsObjectNameEXT) vkGetInstanceProcAddr(context.instance, "vkSetDebugUtilsObjectNameEXT"))
(context.device, &nameInfo);

*/