#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"


namespace mini::vk
{

    struct Default_Sync
    {
        VkDevice device;

        mini::vk::VkArray<VkSemaphore, 4> imageAquired; 
        mini::vk::VkArray<VkSemaphore, 4> renderDone; 
        mini::vk::VkArray<VkFence, 4> fences; 


        Default_Sync(Context& context) : device { context.device }
        {
            imageAquired.count  = context.swapImages.count;
            renderDone.count    = context.swapImages.count;
            fences.count        = context.swapImages.count;

            const VkSemaphoreCreateInfo semaInfo  { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO , nullptr , 0 };
            const VkFenceCreateInfo     fenceInfo { 
                VK_STRUCTURE_TYPE_FENCE_CREATE_INFO , 
                nullptr , 
                VK_FENCE_CREATE_SIGNALED_BIT 
            };

            FOR_VK_ARRAY(imageAquired, i) 
                vkCreateSemaphore(device, &semaInfo, nullptr, &imageAquired[i]);    

            FOR_VK_ARRAY(renderDone, i) 
                vkCreateSemaphore(device, &semaInfo, nullptr, &renderDone[i]);  

            FOR_VK_ARRAY(fences, i) 
                vkCreateFence(device, &fenceInfo, nullptr, &fences[i]);
        }

        ~Default_Sync()
        {
            FOR_VK_ARRAY(imageAquired, i) vkDestroySemaphore(device, imageAquired[i], nullptr);
            FOR_VK_ARRAY(renderDone, i)   vkDestroySemaphore(device, renderDone[i], nullptr);
            FOR_VK_ARRAY(fences, i)       vkDestroyFence(device, fences[i], nullptr);
        }

    };

}//ns