//https://github.com/awwdev

#pragma once

#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Ctors.hpp"
#include "mini/Vulkan/Resources.hpp"
#include "mini/Resources/HostResources.hpp"
#include "mini/Scene/Scene.hpp"
#include "mini/Utils/Vertex.hpp"
#include "mini/Resources/TextMesh.hpp"

namespace mini::vk
{
    struct Renderer
    {
        Context         context;
        VkResources       resources;
        Commands        commands;
        Synchronization sync;

        uint32_t currentFrame = 0;
        

        Renderer(const vk::WindowHandle& wndHandle, hostRes::HostResources& hostResources)
            : context   { wndHandle }
            , resources { context.device }
        {
            //context.Create(wndHandle);
            sync.Create(context);
            commands.Create(context);
            resources.Create(context, hostResources, commands);
        }


        inline void RecreateScwapchain()
        {
            vkDeviceWaitIdle(context.device);

            if (!context.RecreateSwapchain())
                return;

            resources.default_renderPass.~Default_RenderPass();
            resources.default_renderPass.Create(context);

            resources.default_pipeline.~Default_Pipeline();
            resources.default_pipeline.Create(
                context, 
                resources.default_shader, 
                resources.default_renderPass, 
                CreatePipelineVertexInputInfo(resources.default_vb));
            //TODO: maybe write Recreate function for pipeline struct (so it would stroe all its refs)

            commands.~Commands();
            commands.Create(context);
        }


        inline void RecordCommands(const uint32_t cmdBufferIdx, const double dt, const app::scene::Scene& scene)
        {
            auto& cmdBuffer = commands.cmdBuffers[cmdBufferIdx];

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
            vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.default_pipeline.layout, 0, 1, &resources.default_shader.sets[cmdBufferIdx], 0, 0); 
            VkDeviceSize vboOffsets { 0 };
            
            //TODO: find another place where we change host uniforms and host visible stuff
            const auto blockPtr = res::CreateVerticesFromText("H");
            const auto& verts   = blockPtr.Get();
            resources.default_vb.Store(verts.Data(), verts.Count());

            vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &resources.default_vb.buffer.buffer, &vboOffsets);
            for(auto i=0; i<1; ++i) vkCmdDraw(cmdBuffer, 6, 1, 0, 0); //!stress test (increase max)
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

            const auto submitInfo = SubmitInfo(sync.imageAcquired[currentFrame], sync.imageFinished[currentFrame], commands.cmdBuffers[imageIndex]);
            VK_CHECK(vkQueueSubmit(context.queue, 1, &submitInfo, sync.fences[currentFrame]));

            const auto presentInfo = PresentInfo(sync.imageFinished[currentFrame], context.swapchain, imageIndex);
            VK_CHECK(vkQueuePresentKHR(context.queue, &presentInfo));

            currentFrame = (currentFrame + 1) % (context.swapImages.count - 1);
        }
    };

}//ns