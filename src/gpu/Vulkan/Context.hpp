//https://github.com/awwdev

#pragma once

#include "com/Types.hpp"
#include "dbg/Logger.hpp"
#include "dbg/Assert.hpp"
#include "com/box/SimpleArray.hpp"

#define VK_USE_PLATFORM_WIN32_KHR
#include "../ext/include/vulkan.h"

namespace rpg::gpu::vuk {

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

struct WindowHandle
{
    void* ptr1; //win: hinstance
    void* ptr2; //win: hwnd
};

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void*)
{
    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        dbg::LogWarning(pCallbackData->pMessage);

    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        rpg::dbg::Assert(false, pCallbackData->pMessage);

    return VK_FALSE;
}

//!GLOBAL REFERENCE
struct Context;
inline Context* g_contextPtr = nullptr;

struct Context
{
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;

    VkPhysicalDevice physical;
    VkPhysicalDeviceProperties physicalProps;
    VkPhysicalDeviceMemoryProperties physicalMemProps;

    uint32_t queueIndex;
    VkQueue  queue;
    VkDevice device;

    VkSurfaceKHR surface;
    VkSurfaceCapabilitiesKHR surfaceCapabilities;

    const VkFormat format = VK_FORMAT_B8G8R8A8_SRGB;//VK_FORMAT_B8G8R8A8_UNORM;
    const VkColorSpaceKHR COLOR_SPACE = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    const VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
    //const VkPresentModeKHR presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
    //const VkPresentModeKHR presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
    //const VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
    
    VkSwapchainKHR swapchain;
    com::SimpleArrayVk<VkImage, 4>     swapImages;
    com::SimpleArrayVk<VkImageView, 4> swapImageViews;


    void Create(const WindowHandle& wndHandle)
    {
        g_contextPtr = this;
        CreateInstance();
        CreatePhysical();
        CreateLogicalDevice();
        CreateSurface(wndHandle);
        CreateSwapchain();
    }

    void CreateInstance()
    {
        const VkApplicationInfo appInfo {
            .sType                  = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext                  = nullptr,
            .pApplicationName       = nullptr,
            .applicationVersion     = 0,
            .pEngineName            = nullptr,
            .engineVersion          = 0,
            .apiVersion             = VK_API_VERSION_1_0
        };

        chars_t layers[] {
            "VK_LAYER_KHRONOS_validation"
        };

        chars_t extensions[] {
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
            VK_KHR_SURFACE_EXTENSION_NAME,
            VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
        };


        const VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo {
            .sType                  = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .pNext                  = nullptr,
            .flags                  = 0,
            .messageSeverity        = 
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType            = 
                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback        = DebugCallback,
            .pUserData              = nullptr
        };

        const VkInstanceCreateInfo instInfo {
            .sType                  = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext                  = &debugCreateInfo, //so instance creation messages are handled
            .flags                  = 0,
            .pApplicationInfo       = &appInfo,
            .enabledLayerCount      = ArrayCount(layers), 
            .ppEnabledLayerNames    = layers,
            .enabledExtensionCount  = ArrayCount(extensions),
            .ppEnabledExtensionNames = extensions
        };

        VkCheck(vkCreateInstance(&instInfo, nullptr, &instance));

        ((PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"))
            (instance, &debugCreateInfo, nullptr, &debugMessenger);

        //VkArray<VkLayerProperties, 4> layerProps;
        //VkCheck(vkEnumerateInstanceLayerProperties(&layerProps.count, layerProps.data));
        //for(auto i = 0; i < layerProps.count; ++i)
        //{
        //    dbg::LogInfo(layerProps.data[i].layerName);
        //}
    }

    void CreatePhysical()
    {
        com::SimpleArrayVk<VkPhysicalDevice, 4> physicals;
        VkCheck(vkEnumeratePhysicalDevices(instance, &physicals.count, nullptr));
        VkCheck(vkEnumeratePhysicalDevices(instance, &physicals.count, physicals.data));
        physical = physicals[0];

        com::SimpleArrayVk<VkQueueFamilyProperties, 10> famProps;
        vkGetPhysicalDeviceQueueFamilyProperties(physical, &famProps.count, nullptr);
        vkGetPhysicalDeviceQueueFamilyProperties(physical, &famProps.count, famProps.data);

        for (uint32_t i = 0; i < famProps.count; ++i) {
            if (famProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                queueIndex = i;
                break;
            }
        }
        //dbg::LogInfo("queue Index", context.queueIndex);

        vkGetPhysicalDeviceProperties(physical, &physicalProps);
        vkGetPhysicalDeviceMemoryProperties(physical, &physicalMemProps);

        dbg::LogColor(dbg::ConsoleColors::BrightCyan, "Vulkan physical API version", 
            VK_VERSION_MAJOR(physicalProps.apiVersion),
            VK_VERSION_MINOR(physicalProps.apiVersion), 
            VK_VERSION_PATCH(physicalProps.apiVersion)
        );
    }

    inline void CreateLogicalDevice()
    {
        float priorities { 1.f };
        const VkDeviceQueueCreateInfo queueInfo {
            .sType             = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext             = nullptr,
            .flags             = 0,
            .queueFamilyIndex  = queueIndex,
            .queueCount        = 1,
            .pQueuePriorities  = &priorities
        };

        chars_t physicalExtensions[] {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        VkPhysicalDeviceFeatures deviceFeatures {};
        deviceFeatures.sampleRateShading  = VK_TRUE;
        deviceFeatures.depthBiasClamp     = VK_TRUE;
        deviceFeatures.fillModeNonSolid   = VK_TRUE;
        deviceFeatures.wideLines          = VK_TRUE;
        deviceFeatures.alphaToOne         = VK_FALSE;
        deviceFeatures.samplerAnisotropy  = VK_TRUE;
        deviceFeatures.geometryShader     = VK_TRUE;

        const VkDeviceCreateInfo deviceInfo {
            .sType                      = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext                      = nullptr,
            .flags                      = 0,
            .queueCreateInfoCount       = 1,
            .pQueueCreateInfos          = &queueInfo,
            .enabledLayerCount          = 0,
            .ppEnabledLayerNames        = nullptr,
            .enabledExtensionCount      = ArrayCount(physicalExtensions),
            .ppEnabledExtensionNames    = physicalExtensions,
            .pEnabledFeatures           = &deviceFeatures
        };

        VkCheck(vkCreateDevice(physical, &deviceInfo, nullptr, &device));
        vkGetDeviceQueue(device, queueIndex, 0, &queue);
    }

    void CreateSurface(const WindowHandle& wndHandle)
    {
        const VkWin32SurfaceCreateInfoKHR surfInfo {
            .sType      = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
            .pNext      = nullptr,
            .flags      = 0,
            .hinstance  = (HINSTANCE)wndHandle.ptr1,
            .hwnd       = (HWND)wndHandle.ptr2
        };

        VkCheck(vkCreateWin32SurfaceKHR(instance, &surfInfo, nullptr, &surface));

        VkBool32 supported {}; //todo: check for support
        VkCheck(vkGetPhysicalDeviceSurfaceSupportKHR(physical, queueIndex, surface, &supported));
        VkCheck(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical, surface, &surfaceCapabilities));
    }

    bool RecreateSwapchain()
    {
        VkCheck(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical, surface, &surfaceCapabilities));
        if (surfaceCapabilities.currentExtent.width == 0 || surfaceCapabilities.currentExtent.height == 0)
            return false;

        DestroySwapchain();
        CreateSwapchain();

        return true;
    }

    void CreateSwapchain()
    {
        const VkSwapchainCreateInfoKHR swapInfo {
            .sType                  = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext                  = nullptr,
            .flags                  = 0,
            .surface                = surface,
            .minImageCount          = surfaceCapabilities.minImageCount + 1,
            .imageFormat            = format,
            .imageColorSpace        = COLOR_SPACE,
            .imageExtent            = surfaceCapabilities.currentExtent,
            .imageArrayLayers       = 1,
            .imageUsage             = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .imageSharingMode       = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount  = 0,
            .pQueueFamilyIndices    = nullptr,
            .preTransform           = surfaceCapabilities.currentTransform,
            .compositeAlpha         = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode            = presentMode,
            .clipped                = VK_FALSE,
            .oldSwapchain           = nullptr
        };

        VkCheck(vkCreateSwapchainKHR(device, &swapInfo, nullptr, &swapchain));
        //expected to pass count first and get actual count:
        VkCheck(vkGetSwapchainImagesKHR(device, swapchain, &swapImages.count, nullptr));
        VkCheck(vkGetSwapchainImagesKHR(device, swapchain, &swapImages.count, swapImages.data));

        swapImageViews.count = swapImages.count;
        //dbg::LogInfo("swapchain count", images.count);
        for (u32 i=0; i<swapImages.count; ++i) {
            const VkImageViewCreateInfo viewInfo {
                .sType      = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .pNext      = nullptr,
                .flags      = 0,
                .image      = swapImages[i],
                .viewType   = VK_IMAGE_VIEW_TYPE_2D,
                .format     = format,
                .components = {
                    .r = VK_COMPONENT_SWIZZLE_R,
                    .g = VK_COMPONENT_SWIZZLE_G,
                    .b = VK_COMPONENT_SWIZZLE_B,
                    .a = VK_COMPONENT_SWIZZLE_A
                },
                .subresourceRange = {
                    .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel   = 0,
                    .levelCount     = 1,
                    .baseArrayLayer = 0,
                    .layerCount     = 1
                }
            };
            VkCheck(vkCreateImageView(device, &viewInfo, nullptr, &swapImageViews[i]));
        }
    }

    void DestroySwapchain()
    {
        FOR_SIMPLE_ARRAY(swapImageViews, i) 
            vkDestroyImageView(device, swapImageViews[i], nullptr);
        swapImageViews.ResetCount();
        vkDestroySwapchainKHR(device, swapchain, nullptr);
    }

    ~Context()
    {
        Destroy();
    }

    void Destroy()
    {
        FOR_SIMPLE_ARRAY(swapImageViews, i) 
            vkDestroyImageView(device, swapImageViews[i], nullptr);
        swapImageViews.ResetCount();
        vkDestroySwapchainKHR(device, swapchain, nullptr);
        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyDevice(device, nullptr);
        ((PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"))(instance, debugMessenger, nullptr);
        vkDestroyInstance(instance, nullptr);
        g_contextPtr = nullptr;
    }

};

}//ns
