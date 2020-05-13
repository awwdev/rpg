#pragma once
#include "mini/Vulkan/Core.hpp"

namespace mini::vk
{
    struct Context
    {
        VkInstance          instance;
        VkPhysicalDevice    physical;
        VkDevice            device;
    };

    struct WindowData
    {
        void* data1;
        void* data2;
    };


    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {

        ERR(pCallbackData->pMessage);

        return VK_FALSE;
    }


    inline Context CreateContext(const WindowData& wndData)
    {
        //? INSTANCE

        const char* layers[] {
            "VK_LAYER_KHRONOS_validation"
        };

        const char* extensions[] {
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
            VK_KHR_SURFACE_EXTENSION_NAME,
            VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
        };

        const VkApplicationInfo appInfo {
            .sType                  = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext                  = nullptr,
            .pApplicationName       = "mini",
            .applicationVersion     = 0,
            .pEngineName            = "mini",
            .engineVersion          = 0,
            .apiVersion             = VK_API_VERSION_1_0
        };

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugCreateInfo.pfnUserCallback = debugCallback;
        debugCreateInfo.pUserData = nullptr; // Optional

        const VkInstanceCreateInfo instInfo {
            .sType                  = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext                  = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo,
            .flags                  = 0,
            .pApplicationInfo       = &appInfo,
            .enabledLayerCount      = ARRAY_COUNT(layers), 
            .ppEnabledLayerNames    = layers,
            .enabledExtensionCount  = ARRAY_COUNT(extensions),
            .ppEnabledExtensionNames = extensions
        };

        VkInstance instance;
        VK_CHECK(vkCreateInstance(&instInfo, nullptr, &instance));

        //? DEBUG

        

        VkDebugUtilsMessengerEXT debugMessenger;
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        func(instance, &debugCreateInfo, nullptr, &debugMessenger);

        //? PHYSICAL DEVICE

        uint32_t count = 0;

        VkPhysicalDevice physicals[4];
        VK_CHECK(vkEnumeratePhysicalDevices(instance, &count, nullptr));
        VK_CHECK(vkEnumeratePhysicalDevices(instance, &count, physicals));
        VkPhysicalDevice physical = physicals[0];

        VkPhysicalDeviceProperties physicalProps;
        VkPhysicalDeviceMemoryProperties physicalMemProps;

        vkGetPhysicalDeviceProperties(physical, &physicalProps);
        vkGetPhysicalDeviceMemoryProperties(physical, &physicalMemProps);

        VkQueueFamilyProperties famProps[12];
        vkGetPhysicalDeviceQueueFamilyProperties(physical, &count, nullptr);
        vkGetPhysicalDeviceQueueFamilyProperties(physical, &count, famProps);

        uint32_t queueIndex = 0;
        for (uint32_t i = 0; i < count; ++i) {
            if (famProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                queueIndex = i;
                break;
            }
        }

        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(physical, &props);
        std::cout << "physical vulkan ver " 
            <<        VK_VERSION_MAJOR(props.apiVersion) 
            << "." << VK_VERSION_MINOR(props.apiVersion) 
            << "." << VK_VERSION_PATCH(props.apiVersion) 
            << "\n";


        //? LOGICAL DEVICE

        float priorities { 1.f };
        const VkDeviceQueueCreateInfo queueInfo {
            .sType             = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext             = nullptr,
            .flags             = 0,
            .queueFamilyIndex  = queueIndex,
            .queueCount        = 1,
            .pQueuePriorities  = &priorities
        };

        const char* physicalExtensions[] {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        VkPhysicalDeviceFeatures deviceFeatures {};
        deviceFeatures.samplerAnisotropy = VK_TRUE;
        deviceFeatures.fillModeNonSolid = VK_TRUE;
        deviceFeatures.wideLines = VK_TRUE;

        const VkDeviceCreateInfo deviceInfo{
            .sType                      = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext                      = nullptr,
            .flags                      = 0,
            .queueCreateInfoCount       = 1,
            .pQueueCreateInfos          = &queueInfo,
            .enabledLayerCount          = 0,
            .ppEnabledLayerNames        = nullptr,
            .enabledExtensionCount      = ARRAY_COUNT(physicalExtensions),
            .ppEnabledExtensionNames    = physicalExtensions,
            .pEnabledFeatures           = &deviceFeatures
        };

        VkQueue queue;
        VkDevice device;
        VK_CHECK(vkCreateDevice(physical, &deviceInfo, nullptr, &device));
        vkGetDeviceQueue(device, queueIndex, 0, &queue);


        //? SURFACE

        const VkWin32SurfaceCreateInfoKHR surfInfo {
            .sType      = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
            .pNext      = nullptr,
            .flags      = 0,
            .hinstance  = (HINSTANCE)wndData.data2,
            .hwnd       = (HWND)wndData.data1
        };

        VkSurfaceKHR surface;
        VK_CHECK(vkCreateWin32SurfaceKHR(instance, &surfInfo, nullptr, &surface));

        VkBool32 supported {}; //todo: check for support
        VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(physical, queueIndex, surface, &supported));

        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical, surface, &surfaceCapabilities));
        //formats = init::getData<VkSurfaceFormatKHR>(&vkGetPhysicalDeviceSurfaceFormatsKHR, physical, surface);
        //presentModes = init::getData<VkPresentModeKHR>(&vkGetPhysicalDeviceSurfacePresentModesKHR, physical, surface);
        

        //? SWAPCHAIN
        
        const VkSwapchainCreateInfoKHR swapInfo {
            .sType                  = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext                  = nullptr,
            .flags                  = 0,
            .surface                = surface,
            .minImageCount          = surfaceCapabilities.minImageCount + 1,
            .imageFormat            = VK_FORMAT_B8G8R8A8_UNORM,
            .imageColorSpace        = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
            .imageExtent            = surfaceCapabilities.currentExtent,
            .imageArrayLayers       = 1,
            .imageUsage             = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .imageSharingMode       = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount  = 0,
            .pQueueFamilyIndices    = nullptr,
            .preTransform           = surfaceCapabilities.currentTransform,
            .compositeAlpha         = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode            = VK_PRESENT_MODE_IMMEDIATE_KHR,
            .clipped                = VK_TRUE,
            .oldSwapchain           = nullptr
        };

        //VkSwapchainKHR swapchain;
        //VK_CHECK(vkCreateSwapchainKHR(device, &swapInfo, nullptr, &swapchain));

       //uint32_t swapchainCount = 0;
       //VK_CHECK(vkGetSwapchainImagesKHR(device, swapchain, &swapchainCount, nullptr));
       //VkImage images[4];
       //VK_CHECK(vkGetSwapchainImagesKHR(device, swapchain, &swapchainCount, images));

       //VkImageView swapchainImageViews[4];
       //for (auto i=0; i<swapchainCount; ++i) {
       //    const VkImageViewCreateInfo viewInfo {
       //        .sType      = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
       //        .pNext      = nullptr,
       //        .flags      = 0,
       //        .image      = images[i],
       //        .viewType   = VK_IMAGE_VIEW_TYPE_2D,
       //        .format     = VK_FORMAT_B8G8R8A8_UNORM,
       //        .components = {
       //            .r = VK_COMPONENT_SWIZZLE_R,
       //            .g = VK_COMPONENT_SWIZZLE_G,
       //            .b = VK_COMPONENT_SWIZZLE_B,
       //            .a = VK_COMPONENT_SWIZZLE_A
       //        },
       //        .subresourceRange = {
       //            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
       //            .baseMipLevel = 0,
       //            .levelCount = 1,
       //            .baseArrayLayer = 0,
       //            .layerCount = 1
       //        }
       //    };
       //    VK_CHECK(vkCreateImageView(device, &viewInfo, nullptr, &swapchainImageViews[i]));
       //}

        return { instance, physical, device };
    }

}//ns