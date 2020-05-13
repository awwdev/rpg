#pragma once
#pragma warning(disable : 26812) //unscoped vk enum

#include "mini/Types.hpp"
#include "mini/Debug/Logger.hpp"

#define VK_USE_PLATFORM_WIN32_KHR
#include "../third/include/vulkan.h"


//#ifdef DEBUG
#   define VK_CHECK(fn) if (const auto err = fn; err != VK_SUCCESS) LOG("VK ERROR", err);
//#else
//#   define VK_CHECK(fn) fn
//#endif // DEBUG