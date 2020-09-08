//https://github.com/awwdev

#pragma once
#include "Vulkan/Core.hpp"
#include "Vulkan/Context.hpp"

namespace rpg::vk
{
    struct Synchronization
    {
        //triple buffering
        vk::VkArray<VkSemaphore, 3> imageAcquired { 0 }; 
        vk::VkArray<VkSemaphore, 3> imageFinished { 0 }; 
        vk::VkArray<VkFence, 3>     inFlight      { 0 };
        vk::VkArray<VkFence, 3>     fences        { 0 }; 

        void Create()
        {
            imageAcquired.count  = g_contextPtr->swapImages.count;
            imageFinished.count  = g_contextPtr->swapImages.count;
            fences.count         = g_contextPtr->swapImages.count;

            FOR_VK_ARRAY(inFlight, i)
                inFlight[i] = VK_NULL_HANDLE;

            const VkSemaphoreCreateInfo semaInfo  
            { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO , nullptr , 0 };
            const VkFenceCreateInfo fenceInfo 
            { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, nullptr, VK_FENCE_CREATE_SIGNALED_BIT };

            FOR_VK_ARRAY(imageAcquired, i) 
                vkCreateSemaphore(g_contextPtr->device, &semaInfo, nullptr, &imageAcquired[i]);    
            FOR_VK_ARRAY(imageFinished, i) 
                vkCreateSemaphore(g_contextPtr->device, &semaInfo, nullptr, &imageFinished[i]);  
            FOR_VK_ARRAY(fences, i) 
                vkCreateFence(g_contextPtr->device, &fenceInfo, nullptr, &fences[i]);
        }

        ~Synchronization()
        {
            FOR_VK_ARRAY(imageAcquired, i) vkDestroySemaphore   (g_contextPtr->device, imageAcquired[i], nullptr);
            FOR_VK_ARRAY(imageFinished, i) vkDestroySemaphore   (g_contextPtr->device, imageFinished[i], nullptr);
            FOR_VK_ARRAY(fences, i)        vkDestroyFence       (g_contextPtr->device, fences[i], nullptr);
        }

    };

}//ns