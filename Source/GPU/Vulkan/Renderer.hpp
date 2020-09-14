//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/_Old/Resources/Resources.hpp"
#include "Resources/CpuResources.hpp"
#include "App/Scene.hpp"
#include "GPU/RenderData.hpp"
#include "Common/Structs.hpp"
#include "Common/Container/String.hpp"
#include "Window/WindowEvents.hpp"
#include "Common/DeltaTime.hpp"

#include "GPU/Vulkan/_Old/Transfer/RenderCommands.hpp"
#include "GPU/Vulkan/_Old/Transfer/UpdateGpuResources.hpp"

#include "GPU/Vulkan/States/States.hpp"

namespace rpg::gpu::vuk {

struct Renderer
{
    Context         context;
    VkResources     resources;
    States          states;
    Commands        commands;
    Synchronization sync;
    uint32_t        currentFrame = 0;
    
    Renderer(const vuk::WindowHandle& wndHandle, res::HostResources& hostResources)
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

        commands.Clear();
        commands.Create();
    }

    void Render(const double dt, app::GameScene& scene, res::HostResources& hostRes)
    {
        if (wnd::glo::resizeState != wnd::glo::ResizeState::None){
            if (wnd::glo::resizeState == wnd::glo::ResizeState::End)
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
        VkCheck(vkResetFences(context.device, 1, &sync.fences[currentFrame]));

        //!UPDATE GPU RESOURCES AND RECORD COMMANDS----------
        states.Update();
        states.Record();
        //UpdateVkResources_GameScene(resources, scene, hostRes, dt, commands); //TODO: depends on scene
        //RecordCommands(resources, commands, imageIndex, dt, scene);
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
        VkCheck(vkQueueSubmit(context.queue, 1, &submitInfo, sync.fences[currentFrame]));

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
        VkCheck(vkQueuePresentKHR(context.queue, &presentInfo));

        currentFrame = (currentFrame + 1) % (context.swapImages.count - 1);
    }
};

}//ns