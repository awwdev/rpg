//https://github.com/awwdev

#pragma once
#pragma warning(disable : 26812) //unscoped vk enum

#include "mini/Utils/Types.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Debug/Assert.hpp"

#define VK_USE_PLATFORM_WIN32_KHR
#include "../third/include/vulkan.h"

namespace mini::vk {

    enum class VkErrorHandling { None, Warn, Assert };
    constexpr auto VK_ERROR_HANDLING = VkErrorHandling::Warn;

    inline void VkCheck(const VkResult result)
    {
        if constexpr(VK_ERROR_HANDLING == VkErrorHandling::Assert){
            if (result != VK_SUCCESS)
                dbg::Assert(false, result);
            return;
        }
        if constexpr(VK_ERROR_HANDLING == VkErrorHandling::Warn){
            if (result != VK_SUCCESS)
                dbg::LogWarning("VK RESULT", result); 
            return;
        }
    }

}//NS

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

//TODO: remove this 
namespace mini::vk 
{
    #define FOR_VK_ARRAY(arr, i) for(u32 i = 0; i < arr.count; ++i)

    template<class T, u32 N>
    struct VkArray
    {
        static constexpr auto CAPACITY = N;
        VkArray(const u32 pCount = 0) : count { pCount } { ; }

        u32 count;
        T   data[N];

        T&       operator[](const u32 i)       { return data[i]; }
        const T& operator[](const u32 i) const { return data[i]; }

        template<class... CtorArgs>
        void Append(CtorArgs&&... args)
        {
            //TODO: bounds check?
            data[count] = { std::forward<CtorArgs>(args)... };
            ++count;
        }
    };
}