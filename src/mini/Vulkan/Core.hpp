//https://github.com/awwdev

#pragma once
#pragma warning(disable : 26812) //unscoped vk enum

#include "mini/Types.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Debug/Assert.hpp"

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
#define FOR_VK_ARRAY(arr, i) for(uint32_t i = 0; i < arr.count; ++i)

    template<class T, uint32_t N>
    struct VkArray
    {
        T data[N];
        uint32_t count = N;

        T&       operator[](const uint32_t i)       { return data[i]; }
        const T& operator[](const uint32_t i) const { return data[i]; }
    };
}



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