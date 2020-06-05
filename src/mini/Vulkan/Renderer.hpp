//https://github.com/awwdev

#pragma once

#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Ctors.hpp"
#include "mini/Vulkan/Resources.hpp"
#include "mini/Resources/HostResources.hpp"
#include "mini/Scene/Scene.hpp"
#include "mini/Utils/Vertex.hpp"
#include "mini/Resources/Mesh/TextMesh.hpp"
#include "mini/Box/String.hpp"
#include "mini/Window/AppEvents.hpp"

namespace mini::vk
{
    struct Renderer
    {
        Context         context;
        VkResources     resources;
        Commands        commands;
        Synchronization sync;

        uint32_t currentFrame = 0;
        
        //TODO: ref to current window size
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
            resources.default_pipeline.Recreate(context);

            commands.~Commands();
            commands.Create(context);
        }


        inline void UpdateVkResources(const scenes::Scene& scene, const double dt)
        {
            resources.pushConstants.wnd_w = wnd::window_w;
            resources.pushConstants.wnd_h = wnd::window_h;

            box::String<100> fpsStr;
            fpsStr.Set("fps: ");
            char buf[20];
            sprintf_s(buf, "%4.0f", 1/dt);
            fpsStr.Append(buf);

            const auto LETTER_COUNT_MAX = 100;
            auto blockPtr1 = mem::ClaimBlock<box::Array<Vertex,   LETTER_COUNT_MAX * 4>>();
            auto blockPtr2 = mem::ClaimBlock<box::Array<uint32_t, LETTER_COUNT_MAX * 6>>();

            res::CreateVerticesFromText(fpsStr.dataPtr, blockPtr1.Get(), blockPtr2.Get());
            resources.default_vb.Store(blockPtr1.Get().Data(), blockPtr1.Get().Count());
            resources.default_vb.indexBuffer.Store(blockPtr2.Get().Data(), blockPtr2.Get().Count() * sizeof(uint32_t));
        }


        inline void RecordCommands(const uint32_t cmdBufferIdx, const double dt, const scenes::Scene& scene)
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

            vkCmdBeginRenderPass(cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.default_pipeline.pipeline);
            vkCmdPushConstants(cmdBuffer, resources.default_pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(resources.pushConstants), &resources.pushConstants);
            vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.default_pipeline.layout, 0, 1, &resources.default_shader.sets[cmdBufferIdx], 0, 0); 
            VkDeviceSize vboOffsets { 0 };
            vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &resources.default_vb.buffer.buffer, &vboOffsets);
            vkCmdBindIndexBuffer(cmdBuffer, resources.default_vb.indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
            //for(auto i=0; i<1; ++i) vkCmdDraw(cmdBuffer, resources.default_vb.count, 1, 0, 0); //!stress test (increase max)
            vkCmdDrawIndexed(cmdBuffer, resources.default_vb.indexCount, 1, 0, 0, 0); //!stress test (increase max)
            vkCmdEndRenderPass(cmdBuffer);

            VK_CHECK(vkEndCommandBuffer(cmdBuffer));
        }


        inline void Render(const double dt, const scenes::Scene& scene)
        {
            if (wnd::CheckEvent(wnd::EventType::Window_Resize)){
                RecreateScwapchain();
                return;
            }

            //if (context.surfaceCapabilities.currentExtent.width == 0 || 
            //    context.surfaceCapabilities.currentExtent.height == 0)
            //    return;

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
            UpdateVkResources(scene, dt);
            RecordCommands(imageIndex, dt, scene);
            //!--------------------------------------------------

            const auto submitInfo = SubmitInfo(sync.imageAcquired[currentFrame], sync.imageFinished[currentFrame], commands.cmdBuffers[imageIndex]);
            VK_CHECK(vkQueueSubmit(context.queue, 1, &submitInfo, sync.fences[currentFrame]));

            const auto presentInfo = PresentInfo(sync.imageFinished[currentFrame], context.swapchain, imageIndex);
            VK_CHECK(vkQueuePresentKHR(context.queue, &presentInfo));

            currentFrame = (currentFrame + 1) % (context.swapImages.count - 1);
        }
    };

}//ns