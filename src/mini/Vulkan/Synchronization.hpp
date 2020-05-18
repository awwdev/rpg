#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"

#include "mini/Box/Pool.hpp"


namespace mini::vk
{

    struct Default_Sync
    {
        VkDevice device;

        mini::vk::VkArray<VkSemaphore, 4> imageAquired; 
        mini::vk::VkArray<VkSemaphore, 4> renderDone; 

        mini::box::Pool<VkSemaphore, 10> semaphores;
        mini::box::Pool<VkSemaphore, 10> semaphores2;

        //mini::vk::VkArray<VkSemaphore, 20> semaphores; //semaphore pool

        mini::vk::VkArray<VkFence, 4> fences; 
        mini::vk::VkArray<VkFence, 4> inFlight { VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE};


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

            FOR_POOL(semaphores, i) 
                vkCreateSemaphore(device, &semaInfo, nullptr, &semaphores[i]);  

            FOR_POOL(semaphores2, i) 
                vkCreateSemaphore(device, &semaInfo, nullptr, &semaphores2[i]);  
        }

        ~Default_Sync()
        {
            FOR_VK_ARRAY(imageAquired, i) vkDestroySemaphore(device, imageAquired[i], nullptr);
            FOR_VK_ARRAY(renderDone, i)   vkDestroySemaphore(device, renderDone[i], nullptr);
            FOR_VK_ARRAY(fences, i)       vkDestroyFence(device, fences[i], nullptr);
            FOR_POOL(semaphores, i)       vkDestroySemaphore(device, semaphores[i], nullptr);
            FOR_POOL(semaphores, i)       vkDestroySemaphore(device, semaphores2[i], nullptr);
        }

    };

}//ns