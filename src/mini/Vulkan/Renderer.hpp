//https://github.com/awwdev

#pragma once

#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Resources.hpp"
#include "mini/Window/AppEvents.hpp"

#include "app/Scene/Scene.hpp"


namespace mini::vk
{
    //? RENDERER

    struct Renderer
    {
        Context context;
        Resources resources;
        uint32_t currentFrame;
        const VkPipelineStageFlags waitStages;

        //? CTOR

        explicit Renderer(const vk::WindowHandle& wndHandle)
            : context       { wndHandle }
            , resources     { context }
            , currentFrame  { 0 }
            , waitStages    { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT }
        {
        }

        //? HELPER

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

        inline VkSubmitInfo SubmitInfo(VkSemaphore& aquired, VkSemaphore& rendered, VkCommandBuffer& cmdBuffer)
        {
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


        inline void RecreateScwapchain()
        {
            vkDeviceWaitIdle(context.device);

            if (!context.RecreateSwapchain())
                return;

            resources.default_renderPass.~Default_RenderPass();
            resources.default_renderPass.Create(context);

            resources.default_pipeline.~Default_Pipeline();
            resources.default_pipeline.Create(context, resources.default_shader, resources.default_renderPass);

            resources.commands.~Commands();
            resources.commands.Create(context);
        }

        //? RENDERING AND COMMANDS

        inline void RecordCommands(const uint32_t cmdBufferIdx, const double dt, const app::scene::Scene& scene)
        {
            auto& cmdBuffer = resources.commands.cmdBuffers[cmdBufferIdx];

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

            //! stuff from lots of places, so more functions needed
            vkCmdBeginRenderPass(cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.default_pipeline.pipeline);
            static float counter = 0; counter += 1.f * dt; //!testing
            float values = std::sinf(counter) * 0.5f;
            vkCmdPushConstants(cmdBuffer, resources.default_pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(float), &values);
            for(auto i=0; i<100; ++i) vkCmdDraw(cmdBuffer, 3, 1, 0, 0); //!stress test (increase max)
            vkCmdEndRenderPass(cmdBuffer);

            VK_CHECK(vkEndCommandBuffer(cmdBuffer));
        }


        inline void Render(const double dt, const app::scene::Scene& scene)
        {
            if (wnd::CheckEvent(wnd::EventType::Window_Resize)){
                RecreateScwapchain();
                return;
            }

            if (context.surfaceCapabilities.currentExtent.width == 0 || 
                context.surfaceCapabilities.currentExtent.height == 0)
                return;


            auto& sync = resources.synchronization;

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

            //!RECORD COMMANDS----------
            RecordCommands(imageIndex, dt, scene);
            //!-------------------------

            const auto submitInfo = SubmitInfo(sync.imageAcquired[currentFrame], sync.imageFinished[currentFrame], resources.commands.cmdBuffers[imageIndex]);
            VK_CHECK(vkQueueSubmit(context.queue, 1, &submitInfo, sync.fences[currentFrame]));

            const auto presentInfo = PresentInfo(sync.imageFinished[currentFrame], context.swapchain, imageIndex);
            VK_CHECK(vkQueuePresentKHR(context.queue, &presentInfo));

            currentFrame = (currentFrame + 1) % (context.swapImages.count - 1);
        }
    };

}//ns












/*
//stutters under workload but better fps since cpu does not stall 

inline int acquired = 0;
inline int semaphoreImgMapping[3] = {-1, -1, -1}; //triple buffering


inline void Render2(Context& context, Resources& resources, const double dt)
    {
        auto& sync = resources.default_sync;

        //for (auto i = acquired; i < maxImgs - 1; ++i)
        if (acquired < context.swapImages.count - 1)
        {
            uint32_t imageIndex = 0;
            const auto freeSemaphore = sync.semaphores_acquire.Get(); //this does not flag the semaphore as used!

            const auto res = vkAcquireNextImageKHR(
                context.device, 
                context.swapchain, 
                0, 
                sync.semaphores_acquire[freeSemaphore], 
                VK_NULL_HANDLE, 
                &imageIndex
            );

            if (res == VK_SUCCESS)
            {
                ++acquired;
                sync.semaphores_acquire.Set<true>(freeSemaphore); //now flag it as used
                semaphoreImgMapping[imageIndex] = freeSemaphore;

                //LOG_VAR(freeSemaphore);
                //LOG_VAR(imageIndex);
            }
            else 
            {
                //!bad surface check (on resize)
                WARN("vkAcquireNextImageKHR", "no success");
            }
        }

        for(uint32_t i=0; i<ARRAY_COUNT(semaphoreImgMapping); ++i)
        {
            if (semaphoreImgMapping[i] == -1)
                continue;

            if(vkWaitForFences(context.device, 1, &sync.fences_submit[i], VK_FALSE, 0) != VK_SUCCESS)
                continue;
            
            VK_CHECK(vkResetFences(context.device, 1, &sync.fences_submit[i]));
            const auto semaIdx = semaphoreImgMapping[i];
            semaphoreImgMapping[i] = -1;
            sync.semaphores_acquire.Set<false>(semaIdx);
            --acquired;

            //!RECORD COMMANDS----------
            RecordCommands(context, resources, i, dt);
            //!-------------------------

            const auto submitInfo = SubmitInfo(sync.semaphores_acquire[semaIdx], sync.semaphores_render[semaIdx], resources.default_commands.cmdBuffers[i]);
            VK_CHECK(vkQueueSubmit(context.queue, 1, &submitInfo, sync.fences_submit[i]));

            const auto presentInfo = PresentInfo(sync.semaphores_render[semaIdx], context.swapchain, i);
            VK_CHECK(vkQueuePresentKHR(context.queue, &presentInfo));
        }
    }
*/