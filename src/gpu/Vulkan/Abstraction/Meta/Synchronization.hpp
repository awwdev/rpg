//https://github.com/awwdev

#pragma once

#include "gpu/Vulkan/Abstraction/Meta/Context.hpp"
#include "com/box/SimpleArray.hpp"

namespace rpg::gpu::vuk {

struct Synchronization
{
    //triple buffering
    com::SimpleArrayVk<VkSemaphore, 3> imageAcquired; 
    com::SimpleArrayVk<VkSemaphore, 3> imageFinished; 
    com::SimpleArrayVk<VkFence, 3>     inFlight;
    com::SimpleArrayVk<VkFence, 3>     fences; 

    void Create()
    {
        imageAcquired.count  = g_contextPtr->swapImages.count;
        imageFinished.count  = g_contextPtr->swapImages.count;
        fences.count         = g_contextPtr->swapImages.count;
        
        inFlight.count = 3;
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
        Destroy();
    }

    void Destroy()
    {
        FOR_SIMPLE_ARRAY(imageAcquired, i) vkDestroySemaphore   (g_contextPtr->device, imageAcquired[i], nullptr);
        FOR_SIMPLE_ARRAY(imageFinished, i) vkDestroySemaphore   (g_contextPtr->device, imageFinished[i], nullptr);
        FOR_SIMPLE_ARRAY(fences, i)        vkDestroyFence       (g_contextPtr->device, fences[i], nullptr);
        imageAcquired.ResetCount();
        imageFinished.ResetCount();
        fences.ResetCount();
    }

};

}//ns