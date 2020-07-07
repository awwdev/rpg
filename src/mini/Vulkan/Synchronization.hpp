//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"

namespace mini::vk
{
    struct Synchronization
    {
        //triple buffering
        box::POD_Array<VkSemaphore, 3> imageAcquired { 0 }; 
        box::POD_Array<VkSemaphore, 3> imageFinished { 0 }; 
        box::POD_Array<VkFence, 3>     inFlight      { 0 };
        box::POD_Array<VkFence, 3>     fences        { 0 }; 

        void Create()
        {
            imageAcquired.count  = g_contextPtr->swapImages.count;
            imageFinished.count  = g_contextPtr->swapImages.count;
            fences.count         = g_contextPtr->swapImages.count;

            FOR_SIMPLE_ARRAY(inFlight, i)
                inFlight[i] = VK_NULL_HANDLE;

            const VkSemaphoreCreateInfo semaInfo  
            { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO , nullptr , 0 };
            const VkFenceCreateInfo fenceInfo 
            { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, nullptr, VK_FENCE_CREATE_SIGNALED_BIT };

            FOR_SIMPLE_ARRAY(imageAcquired, i) 
                vkCreateSemaphore(g_contextPtr->device, &semaInfo, nullptr, &imageAcquired[i]);    
            FOR_SIMPLE_ARRAY(imageFinished, i) 
                vkCreateSemaphore(g_contextPtr->device, &semaInfo, nullptr, &imageFinished[i]);  
            FOR_SIMPLE_ARRAY(fences, i) 
                vkCreateFence(g_contextPtr->device, &fenceInfo, nullptr, &fences[i]);
        }

        ~Synchronization()
        {
            FOR_SIMPLE_ARRAY(imageAcquired, i) vkDestroySemaphore   (g_contextPtr->device, imageAcquired[i], nullptr);
            FOR_SIMPLE_ARRAY(imageFinished, i) vkDestroySemaphore   (g_contextPtr->device, imageFinished[i], nullptr);
            FOR_SIMPLE_ARRAY(fences, i)        vkDestroyFence       (g_contextPtr->device, fences[i], nullptr);
        }

    };

}//ns