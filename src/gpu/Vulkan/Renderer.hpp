//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Meta/Context.hpp"
#include "gpu/Vulkan/States/States.hpp"
#include "gpu/Vulkan/Meta/Synchronization.hpp"

#include "res/_Old/CpuResources.hpp"
#include "res/Resources.hpp"
#include "app/Scene.hpp"
#include "gpu/RenderData/RenderData.hpp"
#include "wnd/WindowEvents.hpp"
#include "com/ThreadPool.hpp"

namespace rpg::gpu::vuk {

struct Renderer
{
    Context         context;
    Commands        commands;
    Synchronization sync;
    States          states;
    uint32_t        currentFrame = 0;

    //com::ThreadPool<4> threadPool;
    
    Renderer(const vuk::WindowHandle& wndHandle, res::CpuResources& cpuRes, res::Resources& res)
        : context {}
        , commands {}
        , sync {}
        , states {}
    {
        context.Create(wndHandle); //there is a global ptr to vk context
        sync.Create();
        commands.Create();
        states.Create(cpuRes, res, commands.mainCmdPool);
    }

    void RecreateScwapchain(res::CpuResources& cpuRes, res::Resources& res)
    {
        vkDeviceWaitIdle(context.device);
        if (!context.RecreateSwapchain())
            return;

        commands.Destroy();
        commands.Create();
        states.Destroy();
        states.Create(cpuRes, res, commands.mainCmdPool);
    }

    void Render(const double dt, app::GameScene& scene, res::CpuResources& cpuRes, res::Resources& res)
    {
        if (wnd::glo::resizeState != wnd::glo::ResizeState::None){
            if (wnd::glo::resizeState == wnd::glo::ResizeState::End)
                RecreateScwapchain(cpuRes, res);
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
            case VK_ERROR_OUT_OF_DATE_KHR: RecreateScwapchain(cpuRes, res); return; //when?!
            default: return;
        }

        if (sync.inFlight[imageIndex] != VK_NULL_HANDLE) {
            vkWaitForFences(context.device, 1, &sync.inFlight[imageIndex], VK_FALSE, UINT64_MAX); //important to wait
        }
        sync.inFlight[imageIndex] = sync.fences[currentFrame];
        VkCheck(vkResetFences(context.device, 1, &sync.fences[currentFrame]));

        //UPDATE GPU RESOURCES AND RECORD COMMANDS----------
        states.Update(scene.renderData, cpuRes);
        auto cmds = states.Record(commands, imageIndex, scene.renderData, res);
        //--------------------------------------------------

        const VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        const VkSubmitInfo submitInfo {
            .sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext                  = nullptr,
            .waitSemaphoreCount     = 1,
            .pWaitSemaphores        = &sync.imageAcquired[currentFrame],
            .pWaitDstStageMask      = &waitStages,
            .commandBufferCount     = (uint32_t) cmds.Count(),
            .pCommandBuffers        = cmds.Data(),
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