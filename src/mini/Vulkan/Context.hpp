//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"


namespace mini::vk
{
    struct WindowHandle
    {
        void* ptr1; //win: hinstance
        void* ptr2; //win: hwnd
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
            mini::Assert(false, pCallbackData->pMessage);
        }

        return VK_FALSE;
    }


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

        float RATIO = 4 / 3.f; //todo: keep ratio (do not distort)

        const VkFormat format = VK_FORMAT_B8G8R8A8_SRGB;//VK_FORMAT_B8G8R8A8_UNORM;
        const VkColorSpaceKHR COLOR_SPACE = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        const VkPresentModeKHR presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
        //const VkPresentModeKHR presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
        //const VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
        //const VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
        
        
        VkSwapchainKHR swapchain;
        box::POD_Array<VkImage, 4> swapImages { 0 }; //use this to retrieve "swapchain count"
        box::POD_Array<VkImageView, 4> swapImageViews { 0 };


        //? CTOR

        //inline void Create(const WindowHandle& wndHandle)
        explicit Context(const WindowHandle& wndHandle)
        {
            CreateInstance();
            CreatePhysical();
            CreateLogicalDevice();
            CreateSurface(wndHandle);
            CreateSwapchain();
        }

        //? INSTANCE AND DEBUG

        inline void CreateInstance()
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
                .enabledLayerCount      = ARRAY_COUNT(layers), 
                .ppEnabledLayerNames    = layers,
                .enabledExtensionCount  = ARRAY_COUNT(extensions),
                .ppEnabledExtensionNames = extensions
            };

            VK_CHECK(vkCreateInstance(&instInfo, nullptr, &instance));

            ((PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"))
                (instance, &debugCreateInfo, nullptr, &debugMessenger);

            //VkArray<VkLayerProperties, 4> layerProps;
            //VK_CHECK(vkEnumerateInstanceLayerProperties(&layerProps.count, layerProps.data));
            //for(auto i = 0; i < layerProps.count; ++i)
            //{
            //    LOG(layerProps.data[i].layerName);
            //}
        }

        //? PHYSICAL DEVICE

        inline void CreatePhysical()
        {
            box::POD_Array<VkPhysicalDevice, 4> physicals { 4 };
            VK_CHECK(vkEnumeratePhysicalDevices(instance, &physicals.count, physicals.data));
            physical = physicals[0];

            box::POD_Array<VkQueueFamilyProperties, 10> famProps { 10 };
            vkGetPhysicalDeviceQueueFamilyProperties(physical, &famProps.count, famProps.data);

            for (uint32_t i = 0; i < famProps.count; ++i) {
                if (famProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    queueIndex = i;
                    break;
                }
            }
            //LOG("queue Index", context.queueIndex);

            vkGetPhysicalDeviceProperties(physical, &physicalProps);
            vkGetPhysicalDeviceMemoryProperties(physical, &physicalMemProps);

            INFO("Vulkan physical API version", 
                VK_VERSION_MAJOR(physicalProps.apiVersion),
                VK_VERSION_MINOR(physicalProps.apiVersion), 
                VK_VERSION_PATCH(physicalProps.apiVersion)
            );
        }

        //? LOGICAL DEVICE

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
            deviceFeatures.samplerAnisotropy = VK_TRUE;
            deviceFeatures.fillModeNonSolid = VK_TRUE;
            deviceFeatures.wideLines = VK_TRUE;

            const VkDeviceCreateInfo deviceInfo {
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

            VK_CHECK(vkCreateDevice(physical, &deviceInfo, nullptr, &device));
            vkGetDeviceQueue(device, queueIndex, 0, &queue);
        }

        //? SURFACE

        inline void CreateSurface(const WindowHandle& wndHandle)
        {
            const VkWin32SurfaceCreateInfoKHR surfInfo {
                .sType      = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
                .pNext      = nullptr,
                .flags      = 0,
                .hinstance  = (HINSTANCE)wndHandle.ptr1,
                .hwnd       = (HWND)wndHandle.ptr2
            };
    
            VK_CHECK(vkCreateWin32SurfaceKHR(instance, &surfInfo, nullptr, &surface));

            VkBool32 supported {}; //todo: check for support
            VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(physical, queueIndex, surface, &supported));
            VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical, surface, &surfaceCapabilities));
        }

        //? SWAPCHAIN

        inline bool RecreateSwapchain()
        {
            VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical, surface, &surfaceCapabilities));
            if (surfaceCapabilities.currentExtent.width == 0 || surfaceCapabilities.currentExtent.height == 0)
                return false;

            DestroySwapchain();
            CreateSwapchain();

            return true;
        }

        inline void CreateSwapchain()
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
                .clipped                = VK_TRUE,
                .oldSwapchain           = nullptr
            };

            VK_CHECK(vkCreateSwapchainKHR(device, &swapInfo, nullptr, &swapchain));
            //expected to pass count first and get actual count:
            VK_CHECK(vkGetSwapchainImagesKHR(device, swapchain, &swapImages.count, nullptr));
            VK_CHECK(vkGetSwapchainImagesKHR(device, swapchain, &swapImages.count, swapImages.data));

            swapImageViews.count = swapImages.count;
            //LOG("swapchain count", images.count);
            for (auto i=0; i<swapImages.count; ++i) {
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
                VK_CHECK(vkCreateImageView(device, &viewInfo, nullptr, &swapImageViews[i]));
            }
        }

        inline void DestroySwapchain()
        {
            FOR_SIMPLE_ARRAY(swapImageViews, i) vkDestroyImageView(device, swapImageViews[i], nullptr);
            vkDestroySwapchainKHR(device, swapchain, nullptr);
        }

        ~Context()
        {
            FOR_SIMPLE_ARRAY(swapImageViews, i) vkDestroyImageView(device, swapImageViews[i], nullptr);
            vkDestroySwapchainKHR(device, swapchain, nullptr);
            vkDestroySurfaceKHR(instance, surface, nullptr);
            vkDestroyDevice(device, nullptr);
            ((PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"))(instance, debugMessenger, nullptr);
            vkDestroyInstance(instance, nullptr);
        }
    };

}//ns