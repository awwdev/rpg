//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Resources.hpp"
#include "mini/Resources/HostResources.hpp"
#include "mini/App/Scene.hpp"
#include "mini/Rendering/RenderGraph.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/Box/String.hpp"
#include "mini/Window/WindowEvents.hpp"
#include "mini/Utils/DeltaTime.hpp"

#include "mini/Vulkan/Rendering/RenderCommands.hpp"
#include "mini/Vulkan/Rendering/UpdateResources.hpp"

namespace mini::vk {

struct VkRenderer
{
    Context         context;
    VkResources     resources;
    Commands        commands;
    Synchronization sync;
    uint32_t        currentFrame = 0;
    
    VkRenderer(const vk::WindowHandle& wndHandle, res::HostResources& hostResources)
    {
        context.Create(wndHandle); //there is a global ptr to vk context
        sync.Create();
        commands.Create();
        resources.Create(hostResources, commands.cmdPool);
    }

    void RecreateScwapchain()
    {
        vkDeviceWaitIdle(context.device);
        if (!context.RecreateSwapchain())
            return;

        resources.RecreateSwapchain(commands.cmdPool);

        commands.~Commands();
        commands.Create();
    }

    void Render(const double dt, app::GameScene& scene, res::HostResources& hostRes)
    {
        if (wnd::global::events[wnd::Window_Resize] == wnd::Set){
            RecreateScwapchain();
            return;
        } //checking for size==0 is done previously (will also pause other logic)

        if (vkWaitForFences(context.device, 1, &sync.fences[currentFrame], VK_FALSE, 0) != VK_SUCCESS)
            return;

        uint32_t imageIndex = 0;
        const auto acquireRes = vkAcquireNextImageKHR(
            context.device, 
            context.swapchain, 
            0, 
            sync.imageAcquired[currentFrame], 
            VK_NULL_HANDLE, 
            &imageIndex
        ); 

        switch(acquireRes)
        {
            case VK_SUCCESS: break;
            case VK_ERROR_OUT_OF_DATE_KHR: RecreateScwapchain(); return; //when?!
            default: return;
        }

        if (sync.inFlight[imageIndex] != VK_NULL_HANDLE) {
            vkWaitForFences(context.device, 1, &sync.inFlight[imageIndex], VK_FALSE, UINT64_MAX); //important to wait
        }
        sync.inFlight[imageIndex] = sync.fences[currentFrame];
        VK_CHECK(vkResetFences(context.device, 1, &sync.fences[currentFrame]));

        //!UPDATE GPU RESOURCES AND RECORD COMMANDS----------
        UpdateVkResources_GameScene(resources, scene, hostRes, dt); //TODO: depends on scene
        RecordCommands(resources, commands, imageIndex, dt, scene);
        //!--------------------------------------------------

        const VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        const VkSubmitInfo submitInfo {
            .sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext                  = nullptr,
            .waitSemaphoreCount     = 1,
            .pWaitSemaphores        = &sync.imageAcquired[currentFrame],
            .pWaitDstStageMask      = &waitStages,
            .commandBufferCount     = 1,
            .pCommandBuffers        = &commands.cmdBuffers[imageIndex],
            .signalSemaphoreCount   = 1,
            .pSignalSemaphores      = &sync.imageFinished[currentFrame],
        };
        VK_CHECK(vkQueueSubmit(context.queue, 1, &submitInfo, sync.fences[currentFrame]));

        const VkPresentInfoKHR presentInfo {
            .sType                  = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext                  = nullptr,
            .waitSemaphoreCount     = 1,
            .pWaitSemaphores        = &sync.imageFinished[currentFrame],
            .swapchainCount         = 1,
            .pSwapchains            = &context.swapchain,
            .pImageIndices          = &imageIndex,
            .pResults               = nullptr
        };
        VK_CHECK(vkQueuePresentKHR(context.queue, &presentInfo));

        currentFrame = (currentFrame + 1) % (context.swapImages.count - 1);
    }
};

}//ns