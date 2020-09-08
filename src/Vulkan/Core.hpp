//https://github.com/awwdev

#pragma once

#include "Utils/Types.hpp"
#include "Debug/Logger.hpp"
#include "Debug/Assert.hpp"

#define VK_USE_PLATFORM_WIN32_KHR
#include "../third/include/vulkan.h"
		
namespace rpg::vk {

enum class VkErrorHandling { None, Warn, Assert };
constexpr auto VK_ERROR_HANDLING = VkErrorHandling::Warn;

inline void VkCheck(const VkResult result)
{
    if constexpr(VK_ERROR_HANDLING == VkErrorHandling::Assert){
        if (result != VK_SUCCESS)
            dbg::Assert(false, result);
    }
    if constexpr(VK_ERROR_HANDLING == VkErrorHandling::Warn){
        if (result != VK_SUCCESS)
            dbg::LogWarning("VK RESULT", result); 
    }
}

 #define FOR_VK_ARRAY(arr, i) for(u32 i = 0; i < arr.count; ++i)

template<class T, auto N>
struct VkArray
{
    VkArray(const idx_t pCount = 0) 
        : count { pCount } 
    { ; }

    idx_t count;
    T     data[N];

    T&       operator[](const idx_t i)       { return data[i]; }
    const T& operator[](const idx_t i) const { return data[i]; }
};

}//NS

/*
//use this to give a resource a name to view in renderdoc
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