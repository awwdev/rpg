#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"


namespace mini::vk
{

    struct Default_Semaphores
    {
        VkDevice device;

        mini::vk::VkArray<VkSemaphore, 4> imageAquired; 
        mini::vk::VkArray<VkSemaphore, 4> renderDone; 

        Default_Semaphores(Context& context) : device { context.device }
        {
            imageAquired.count  = context.swapImages.count;
            renderDone.count    = context.swapImages.count;

            const VkSemaphoreCreateInfo semaInfo { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO , nullptr , 0 };

            FOR_VK_ARRAY(imageAquired, i) 
                vkCreateSemaphore(device, &semaInfo, nullptr, &imageAquired[i]);    

            FOR_VK_ARRAY(renderDone, i) 
                vkCreateSemaphore(device, &semaInfo, nullptr, &renderDone[i]);    
        }

        ~Default_Semaphores()
        {
            FOR_VK_ARRAY(imageAquired, i) vkDestroySemaphore(device, imageAquired[i], nullptr);
            FOR_VK_ARRAY(renderDone, i)   vkDestroySemaphore(device, renderDone[i], nullptr);
        }

    };

}//ns