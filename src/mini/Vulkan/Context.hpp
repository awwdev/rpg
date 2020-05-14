#pragma once
#include "mini/Vulkan/Core.hpp"

namespace mini::vk
{
    struct Context
    {
        VkInstance                          instance;
        VkDebugUtilsMessengerEXT            debugMessenger;

        VkPhysicalDevice                    physical;
        VkPhysicalDeviceProperties          physicalProps;
        VkPhysicalDeviceMemoryProperties    physicalMemProps;

        uint32_t queueIndex;


        inline void Destroy()
        {
            ((PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"))(instance, debugMessenger, nullptr);
            vkDestroyInstance(instance, nullptr);
        }
    };


    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
            WARN(pCallbackData->pMessage);

        if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        {
            ERR(pCallbackData->pMessage);
            system("pause");
        }

        return VK_FALSE;
    }


    //? INSTANCE AND DEBUG

    inline void CreateInstance(Context& context)
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
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME
        };


        const VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo {
            .sType                  = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .pNext                  = nullptr,
            .flags                  = 0,
            .messageSeverity        = 
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
            .enabledLayerCount      = ARRAY_COUNT(layers), 
            .ppEnabledLayerNames    = layers,
            .enabledExtensionCount  = ARRAY_COUNT(extensions),
            .ppEnabledExtensionNames = extensions
        };

        VK_CHECK(vkCreateInstance(&instInfo, nullptr, &context.instance));

        ((PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(context.instance, "vkCreateDebugUtilsMessengerEXT"))
            (context.instance, &debugCreateInfo, nullptr, &context.debugMessenger);

        //VkArray<VkLayerProperties, 4> layerProps;
        //VK_CHECK(vkEnumerateInstanceLayerProperties(&layerProps.count, layerProps.data));
        //for(auto i = 0; i < layerProps.count; ++i)
        //{
        //    LOG(layerProps.data[i].layerName);
        //}
    }


    //? PHYSICAL DEVICE

    inline void CreatePhysical(Context& context)
    {
        VkArray<VkPhysicalDevice, 4> physicals;
        VK_CHECK(vkEnumeratePhysicalDevices(context.instance, &physicals.count, physicals.data));
        auto& physical = context.physical = physicals[0];

        VkArray<VkQueueFamilyProperties, 10> famProps;
        vkGetPhysicalDeviceQueueFamilyProperties(physical, &famProps.count, famProps.data);

        for (uint32_t i = 0; i < famProps.count; ++i) {
            if (famProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                context.queueIndex = i;
                break;
            }
        }
        //LOG("queue Index", context.queueIndex);

        vkGetPhysicalDeviceProperties(physical, &context.physicalProps);
        vkGetPhysicalDeviceMemoryProperties(physical, &context.physicalMemProps);
   
        INFO("Vulkan physical API version", 
            VK_VERSION_MAJOR(context.physicalProps.apiVersion),
            VK_VERSION_MINOR(context.physicalProps.apiVersion), 
            VK_VERSION_PATCH(context.physicalProps.apiVersion)
        );
    }


    //? LOGICAL DEVICE

    inline void CreateLogicalDevice(Context& context)
    {

    }


    //? CONTEXT

    inline Context CreateContext()
    {
        Context context;

        CreateInstance(context);
        CreatePhysical(context);
        CreateLogicalDevice(context);

        return context;
    }











    /*

   
    struct WindowData
    {
        void* data1;
        void* data2;
    };



        


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

    */

}//ns