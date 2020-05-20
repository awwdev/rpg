#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"

#include "mini/Box/Pool.hpp"


namespace mini::vk
{

    struct Default_Sync
    {
        VkDevice device;

        //triple buffering
        mini::vk::VkArray<VkSemaphore, 3> imageAcquired; 
        mini::vk::VkArray<VkSemaphore, 3> imageFinished; 
        mini::vk::VkArray<VkFence, 3>     inFlight { VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE};
        mini::vk::VkArray<VkFence, 3>     fences; 


        inline void Create(Context& context)
        {
            device = context.device;

            imageAcquired.count  = context.swapImages.count;
            imageFinished.count  = context.swapImages.count;

            const VkSemaphoreCreateInfo semaInfo  
            { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO , nullptr , 0 };
            const VkFenceCreateInfo fenceInfo 
            { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, nullptr, VK_FENCE_CREATE_SIGNALED_BIT };

            FOR_VK_ARRAY(imageAcquired, i) 
                vkCreateSemaphore(device, &semaInfo, nullptr, &imageAcquired[i]);    
            FOR_VK_ARRAY(imageFinished, i) 
                vkCreateSemaphore(device, &semaInfo, nullptr, &imageFinished[i]);  
            FOR_VK_ARRAY(fences, i) 
                vkCreateFence(device, &fenceInfo, nullptr, &fences[i]);
        }

        ~Default_Sync()
        {
            FOR_VK_ARRAY(imageAcquired, i) vkDestroySemaphore(device, imageAcquired[i], nullptr);
            FOR_VK_ARRAY(imageFinished, i) vkDestroySemaphore(device, imageFinished[i], nullptr);
            FOR_VK_ARRAY(fences, i)        vkDestroyFence(device, fences[i], nullptr);
        }

    };

}//ns






/*
//used for render2()

//triple buffering
mini::box::Pool<VkSemaphore, 3> semaphores_acquire;
mini::box::Pool<VkSemaphore, 3> semaphores_render;
mini::box::Pool<VkFence, 3>     fences_submit;

FOR_POOL(fences_submit, i)      vkDestroyFence(device, fences_submit[i], nullptr);
FOR_POOL(semaphores_acquire, i) vkDestroySemaphore(device, semaphores_acquire[i], nullptr);
FOR_POOL(semaphores_render, i)  vkDestroySemaphore(device, semaphores_render[i], nullptr);
        
FOR_POOL(fences_submit, i) 
    vkCreateFence(device, &fenceInfo, nullptr, &fences_submit[i]);
FOR_POOL(semaphores_acquire, i) 
    vkCreateSemaphore(device, &semaInfo, nullptr, &semaphores_acquire[i]);
FOR_POOL(semaphores_render, i) 
    vkCreateSemaphore(device, &semaInfo, nullptr, &semaphores_render[i]);
*/