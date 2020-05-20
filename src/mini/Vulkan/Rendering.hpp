#pragma once

#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Resources.hpp"

namespace mini::vk
{
    inline VkSubmitInfo SubmitInfo(VkSemaphore& aquired, VkSemaphore& rendered, VkCommandBuffer& cmdBuffer)
    {
        static const VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        return {
            .sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext                  = nullptr,
            .waitSemaphoreCount     = 1,
            .pWaitSemaphores        = &aquired,
            .pWaitDstStageMask      = &waitStages,
            .commandBufferCount     = 1,
            .pCommandBuffers        = &cmdBuffer,
            .signalSemaphoreCount   = 1,
            .pSignalSemaphores      = &rendered,
        };
    }


    inline VkPresentInfoKHR PresentInfo(VkSemaphore& rendered, VkSwapchainKHR& swapchain, uint32_t& imgIndex)
    {
        return {
            .sType                  = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext                  = nullptr,
            .waitSemaphoreCount     = 1,
            .pWaitSemaphores        = &rendered,
            .swapchainCount         = 1,
            .pSwapchains            = &swapchain,
            .pImageIndices          = &imgIndex,
            .pResults               = nullptr
        };
    }


    inline void RecordCommands(Context& context, Resources& resources, const uint32_t cmdBufferIdx)
    {
        auto& cmdBuffer = resources.default_commands.cmdBuffers[cmdBufferIdx];

        auto beginInfo = vk::CreateCmdBeginInfo();
        VK_CHECK(vkBeginCommandBuffer(cmdBuffer, &beginInfo));

        const VkClearValue clears [1] {
            {.color = { 0.1f, 0.1f, 0.1f, 1.0f }},
        };

        const VkRenderPassBeginInfo renderPassInfo {
            .sType          = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext          = nullptr,
            .renderPass     = resources.default_renderPass.renderPass,
            .framebuffer    = resources.default_renderPass.framebuffers[cmdBufferIdx],
            .renderArea     = {
                .offset     = VkOffset2D {0, 0},
                .extent     = context.surfaceCapabilities.currentExtent
            },
            .clearValueCount= ARRAY_COUNT(clears),
            .pClearValues   = clears
        };

        vkCmdBeginRenderPass(cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.default_pipeline.pipeline);
        static float counter = 0; counter += 0.001f; //!testing
        float values = std::sinf(counter) * 0.5f;
        vkCmdPushConstants(cmdBuffer, resources.default_pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(float), &values);
        vkCmdDraw(cmdBuffer, 3, 1, 0, 0);
        vkCmdEndRenderPass(cmdBuffer);

        VK_CHECK(vkEndCommandBuffer(cmdBuffer));
    }


    inline void Render(Context& context, Resources& resources)
    {
        //!todo better not static
        static int acquired = 0;
        static int semaphoreImgMapping[4] = {-1, -1, -1, -1};

        const auto maxImgs = context.swapImages.count;
        auto& sync = resources.default_sync;

        //for (auto i = acquired; i < maxImgs - 1; ++i)
        if (acquired < maxImgs - 1)
        {
            uint32_t imageIndex = 0;
            const auto freeSemaphore = sync.semaphores.Get();
            const auto acquireRes = vkAcquireNextImageKHR(
                context.device, 
                context.swapchain, 
                0, 
                sync.semaphores[freeSemaphore], 
                VK_NULL_HANDLE, 
                &imageIndex
            );

            if (acquireRes == VK_SUCCESS)
            {
                ++acquired;
     
                sync.semaphores.Set<true>(freeSemaphore);
                semaphoreImgMapping[imageIndex] = freeSemaphore;
            }
        }

        for(uint32_t i=0; i<ARRAY_COUNT(semaphoreImgMapping); ++i)
        {
            if (semaphoreImgMapping[i] == -1)
                continue;

            auto imageIndex = i;
            const auto freeSemaphore = semaphoreImgMapping[i]; //name?!

            const auto fenceRes = vkWaitForFences(context.device, 1, &sync.fences[imageIndex], VK_FALSE, 0);
            if(fenceRes != VK_SUCCESS) continue;
            
            VK_CHECK(vkResetFences(context.device, 1, &sync.fences[imageIndex]));
            --acquired;
            sync.semaphores.Set<false>(semaphoreImgMapping[imageIndex]);
            semaphoreImgMapping[i] = -1;

            RecordCommands(context, resources, imageIndex);

            const auto submitInfo = SubmitInfo(sync.semaphores[freeSemaphore], sync.semaphores2[freeSemaphore], resources.default_commands.cmdBuffers[imageIndex]);
            VK_CHECK(vkQueueSubmit(context.queue, 1, &submitInfo, sync.fences[imageIndex]));

            const auto presentInfo = PresentInfo(sync.semaphores2[freeSemaphore], context.swapchain, imageIndex);
            VK_CHECK(vkQueuePresentKHR(context.queue, &presentInfo));
        }
    }


}//ns

/*
        static uint32_t currentFrame = 0;

        const auto res1 = vkWaitForFences(device, 1, &sync.fences[currentFrame], VK_FALSE, 0);
        if (res1 != VK_SUCCESS) continue;

        uint32_t imageIndex = 0;
        const auto acquireRes = vkAcquireNextImageKHR(
            device, 
            swapchain, 
            0, 
            sync.imageAcquired[currentFrame], 
            VK_NULL_HANDLE, 
            &imageIndex
        ); //! check res 
        if (acquireRes != VK_SUCCESS) continue;

        if (sync.inFlight[imageIndex] != VK_NULL_HANDLE) {
            const auto  res2 = vkWaitForFences(device, 1, &sync.inFlight[imageIndex], VK_FALSE, UINT64_MAX);
            if (res2 != VK_SUCCESS) continue;
        }
        sync.inFlight[imageIndex] = sync.fences[currentFrame];
        vkResetFences(device, 1, &sync.fences[currentFrame]);

        record(imageIndex);

        const auto submitInfo = SubmitInfo(sync.imageAcquired[currentFrame], sync.imageFinished[currentFrame], cmds.cmdBuffers[imageIndex]);
        VK_CHECK(vkQueueSubmit(pContext->queue, 1, &submitInfo, pResources->default_sync.fences[currentFrame]));

        const auto presentInfo = PresentInfo(sync.imageFinished[currentFrame], swapchain, imageIndex);
        VK_CHECK(vkQueuePresentKHR(pContext->queue, &presentInfo));

        currentFrame = (currentFrame + 1) % (maxImgs - 1);
        */ 