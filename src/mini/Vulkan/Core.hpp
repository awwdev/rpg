#pragma once
#pragma warning(disable : 26812) //unscoped vk enum

#include "mini/Types.hpp"
#include "mini/Debug/Logger.hpp"

#define VK_USE_PLATFORM_WIN32_KHR
#include "../third/include/vulkan.h"


//#ifdef DEBUG
#define VK_CHECK(fn) \
if (const auto res = fn; res != VK_SUCCESS) \
{ \
    if (res > 0) \
        WARN("VK RESULT", res); \
    else \
        ERR("VK RESULT", res); \
} 
//#else
//#   define VK_CHECK(fn) fn
//#endif // DEBUG

namespace mini::vk
{
    template<class T, auto N>
    struct VkArray
    {
        uint32_t count = N;
        T data[N];

        T&       operator[](const uint32_t i)       { return data[i]; }
        const T& operator[](const uint32_t i) const { return data[i]; }
    };
}