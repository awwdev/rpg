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
            imageAcquired.count  = context.swapImages.count;
            imageFinished.count  = context.swapImages.count;
            fences.count         = context.swapImages.count;

            FOR_SIMPLE_ARRAY(inFlight, i)
                inFlight[i] = VK_NULL_HANDLE;

            const VkSemaphoreCreateInfo semaInfo  
            { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO , nullptr , 0 };
            const VkFenceCreateInfo fenceInfo 
            { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, nullptr, VK_FENCE_CREATE_SIGNALED_BIT };

            FOR_SIMPLE_ARRAY(imageAcquired, i) 
                vkCreateSemaphore(context.device, &semaInfo, nullptr, &imageAcquired[i]);    
            FOR_SIMPLE_ARRAY(imageFinished, i) 
                vkCreateSemaphore(context.device, &semaInfo, nullptr, &imageFinished[i]);  
            FOR_SIMPLE_ARRAY(fences, i) 
                vkCreateFence(context.device, &fenceInfo, nullptr, &fences[i]);
        }

        ~Synchronization()
        {
            FOR_SIMPLE_ARRAY(imageAcquired, i) vkDestroySemaphore   (context.device, imageAcquired[i], nullptr);
            FOR_SIMPLE_ARRAY(imageFinished, i) vkDestroySemaphore   (context.device, imageFinished[i], nullptr);
            FOR_SIMPLE_ARRAY(fences, i)        vkDestroyFence       (context.device, fences[i], nullptr);
        }

    };

}//ns