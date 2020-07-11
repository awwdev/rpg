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
        
        VkRenderer(const vk::WindowHandle& wndHandle, hostRes::HostResources& hostResources)
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
            resources.default_pushConsts.projection = {
                scene.camera.GetMat()
            };

            resources.default_pushConsts.wnd_w = wnd::window_w;
            resources.default_pushConsts.wnd_h = wnd::window_h;

            resources.ui_ubo_array.Clear();
            resources.ui_ubo_array.Append(scene.renderGraph.ubo_ui);

            resources.default_ubo_groups.Clear();
            FOR_ARRAY(scene.renderGraph.default_uboGroups, i){
                const auto& group = scene.renderGraph.default_uboGroups[i];
                const auto& uboArray = scene.renderGraph.default_uboArray;
                resources.default_ubo_groups.AppendGroup(&uboArray[group.begin], group.count); 
            }
            
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
                .renderPass     = resources.ui_renderPass.renderPass,
                .framebuffer    = resources.ui_renderPass.framebuffers[cmdBufferIdx],
                .renderArea     = {
                    .offset     = VkOffset2D {0, 0},
                    .extent     = g_contextPtr->surfaceCapabilities.currentExtent
                },
                .clearValueCount= 0,
                .pClearValues   = nullptr
            };

            const VkClearValue clears_default [] { 
                { .color = { 0.1f, 0.1f, 0.1f, 1.0f } },
                { .depthStencil = { 1.0f, 0 } }
            };
            const VkRenderPassBeginInfo beginInfo_default {
                .sType          = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                .pNext          = nullptr,
                .renderPass     = resources.default_renderPass.renderPass,
                .framebuffer    = resources.default_renderPass.framebuffers[cmdBufferIdx],
                .renderArea     = {
                    .offset     = VkOffset2D {0, 0},
                    .extent     = g_contextPtr->surfaceCapabilities.currentExtent
                },
                .clearValueCount= ARRAY_COUNT(clears_default),
                .pClearValues   = clears_default
            };

            //! does this work fot both pipelines?? (see layout passed) 
            vkCmdPushConstants(cmdBuffer, resources.ui_pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(resources.default_pushConsts), &resources.default_pushConsts);

            //? DEFAULT
            VkDeviceSize offsets = 0;
            vkCmdBeginRenderPass    (cmdBuffer, &beginInfo_default, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.default_pipeline.pipeline);
            vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &resources.default_vbo.dstBuffer.buffer, &offsets);
            vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.default_pipeline.layout, 0, 1, &resources.default_pipeline.sets[cmdBufferIdx], 0, nullptr); 
            const auto instTypeCount = resources.default_ubo_groups.groups.Count(); //ubo group (inst type) congruent to vertex group
            for(u32 i = 0; i < instTypeCount; ++i) {
                const auto vertOff   = resources.default_vbo.vertexGroups[i].begin;
                const auto vertCount = resources.default_vbo.vertexGroups[i].count;
                const auto instOff   = resources.default_ubo_groups.groups[i].begin;
                const auto instCount = resources.default_ubo_groups.groups[i].count;
                vkCmdDraw (cmdBuffer, vertCount, instCount, vertOff, instOff); 
            }
            vkCmdEndRenderPass      (cmdBuffer);
           
            //? TEXT
            vkCmdBeginRenderPass    (cmdBuffer, &beginInfo_ui, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.ui_pipeline.pipeline);
            vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.ui_pipeline.layout, 0, 1, &resources.ui_pipeline.sets[cmdBufferIdx], 0, nullptr); 
            vkCmdDraw               (cmdBuffer, resources.ui_ubo_array.count * 6, 1, 0, 0); 
            vkCmdEndRenderPass      (cmdBuffer);
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
