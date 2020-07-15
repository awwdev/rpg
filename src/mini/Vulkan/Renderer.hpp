//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Ctors.hpp"
#include "mini/Vulkan/Resources.hpp"
#include "mini/Resources/HostResources.hpp"
#include "mini/App/Scene.hpp"
#include "mini/Rendering/RenderGraph.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/Box/String.hpp"
#include "mini/Window/AppEvents.hpp"
#include "mini/Utils/DeltaTime.hpp"
#undef near
#undef far

namespace mini::vk
{
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
            resources.Create(hostResources, commands);
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


        void UpdateVkResources(const app::Scene& scene, const double dt)
        {
            resources.common_pushConsts.projection = {
                scene.camera.GetMat()
            };

            resources.common_pushConsts.wnd_w = wnd::window_w;
            resources.common_pushConsts.wnd_h = wnd::window_h;

            resources.ui.ubo.Clear();
            resources.ui.ubo.Store(scene.renderGraph.ui_ubo);

            resources.default.ubo.Clear();
            resources.default.ubo.Store(scene.renderGraph.default_ubo);
        }

        void RecordCommands(const uint32_t cmdBufferIdx, const double dt, const app::Scene& scene)
        {
            auto& cmdBuffer = commands.cmdBuffers[cmdBufferIdx];
            VkDeviceSize vboOffsets { 0 };
            uint32_t     uboOffsets { 0 };

            auto beginInfo = vk::CreateCmdBeginInfo();
            VK_CHECK(vkBeginCommandBuffer(cmdBuffer, &beginInfo));

            const VkRenderPassBeginInfo beginInfo_ui {
                .sType          = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                .pNext          = nullptr,
                .renderPass     = resources.ui.renderPass.renderPass,
                .framebuffer    = resources.ui.renderPass.framebuffers[cmdBufferIdx],
                .renderArea     = {
                    .offset     = VkOffset2D {0, 0},
                    .extent     = g_contextPtr->surfaceCapabilities.currentExtent
                },
                .clearValueCount= 0,
                .pClearValues   = nullptr
            };

            const VkClearValue clears_default [] { 
                { .color = { 0.1f, 0.1f, 0.1f, 1.0f } },
                { .depthStencil = { 0, 0 } }
            };
            const VkRenderPassBeginInfo beginInfo_default {
                .sType          = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                .pNext          = nullptr,
                .renderPass     = resources.default.renderPass.renderPass,
                .framebuffer    = resources.default.renderPass.framebuffers[cmdBufferIdx],
                .renderArea     = {
                    .offset     = VkOffset2D {0, 0},
                    .extent     = g_contextPtr->surfaceCapabilities.currentExtent
                },
                .clearValueCount= ARRAY_COUNT(clears_default),
                .pClearValues   = clears_default
            };

            //! does this work for both pipelines?? (see layout passed) 
            vkCmdPushConstants(cmdBuffer, resources.ui.pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(resources.common_pushConsts), &resources.common_pushConsts);

            //? DEFAULT
            VkDeviceSize offsets = 0;
            vkCmdBeginRenderPass    (cmdBuffer, &beginInfo_default, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.default.pipeline.pipeline);
            vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &resources.default.vbo.dstBuffer.buffer, &offsets);
            vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.default.pipeline.layout, 0, 1, &resources.default.pipeline.sets[cmdBufferIdx], 0, nullptr); 
            // ubo group idx == vbo group idx == instance
            // 1 ubo group -> 1 inst type (one draw call)
            // 1 ubo group -> N insts
            // vbo needs no group bookkeeping since ubo groups are congruent
            const auto instTypeCount = resources.default.ubo.groups->Count();
            for(u32 i = 0; i < instTypeCount; ++i) {
                const auto vertOff   = resources.default.vbo.vertexGroups[i].begin;
                const auto vertCount = resources.default.vbo.vertexGroups[i].count;
                const auto instOff   = resources.default.ubo.groups->operator[](i).begin;
                const auto instCount = resources.default.ubo.groups->operator[](i).count;
                if (instCount == 0) continue;
                vkCmdDraw (cmdBuffer, vertCount, instCount, vertOff, instOff); 
            }
            vkCmdEndRenderPass      (cmdBuffer);
           
            //? TEXT
            //vkCmdBeginRenderPass    (cmdBuffer, &beginInfo_ui, VK_SUBPASS_CONTENTS_INLINE);
            //vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.ui.pipeline.pipeline);
            //vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.ui.pipeline.layout, 0, 1, &resources.ui.pipeline.sets[cmdBufferIdx], 0, nullptr); 
            //vkCmdDraw               (cmdBuffer, resources.ui.ubo.count * 6, 1, 0, 0); 
            //vkCmdEndRenderPass      (cmdBuffer);
            VK_CHECK(vkEndCommandBuffer(cmdBuffer));
        }


        void Render(const double dt, app::Scene& scene)
        {
            if (wnd::CheckEvent(wnd::EventType::Window_Resize)){
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
