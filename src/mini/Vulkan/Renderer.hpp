//https://github.com/awwdev

#pragma once

#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Ctors.hpp"
#include "mini/Vulkan/Resources.hpp"
#include "mini/Resources/HostResources.hpp"
#include "mini/App/Scene.hpp"
#include "mini/App/IRenderer.hpp"
#include "mini/Utils/Vertex.hpp"
#include "mini/Box/String.hpp"
#include "mini/Window/AppEvents.hpp"

namespace mini::vk
{
    struct VkRenderer : app::IRenderer
    {
        Context         context; //! must come first
        VkResources     resources;
        Commands        commands;
        Synchronization sync;

        uint32_t currentFrame = 0;
        
        VkRenderer(const vk::WindowHandle& wndHandle, hostRes::HostResources& hostResources)
            : IRenderer { hostResources }
            , context   { wndHandle } //! must come first
            , resources { context }
        {
            uniforms.alignment = context.physicalProps.limits.minUniformBufferOffsetAlignment;

            sync.Create(context);
            commands.Create(context);
            resources.Create(context, hostResources, commands);
        }


        inline void RecreateScwapchain()
        {
            vkDeviceWaitIdle(context.device);

            if (!context.RecreateSwapchain())
                return;

            resources.ui_renderPass.~RenderPass();
            CreateRenderPass_UI(context, resources.ui_renderPass);

            resources.ui_pipeline.~Pipeline();
            CreatePipeline_UI(
                context, 
                resources.ui_pipeline,
                resources.ui_shader, 
                resources.ui_renderPass, 
                resources.ui_vbo, 
                resources.ui_ibo
            );
            CreatePipeline_UI_wire(
                context, 
                resources.ui_pipeline_wire,
                resources.ui_shader_wire, 
                resources.ui_renderPass, 
                resources.ui_vbo, 
                resources.ui_ibo
            );

            commands.~Commands();
            commands.Create(context);
        }


        inline void UpdateVkResources(const app::Scene& scene, const double dt)
        {
            resources.ui_pushConst.wnd_w = wnd::window_w;
            resources.ui_pushConst.wnd_h = wnd::window_h;

            resources.ui_vbo.vertexBuffer.Store(vertices.Data(), vertices.Count()  * sizeof(Vertex));
            resources.ui_vbo.indexBuffer.Store (indices.Data(), indices.Count() * sizeof(uint32_t));

            resources.ui_ibo.buffer.Store(uniforms.data, uniforms.CurrentSize());
        }


        inline void RecordCommands(const uint32_t cmdBufferIdx, const double dt, const app::Scene& scene)
        {
            auto& cmdBuffer = commands.cmdBuffers[cmdBufferIdx];
            VkDeviceSize vboOffsets { 0 };
            uint32_t     uboOffsets { 0 };

            auto beginInfo = vk::CreateCmdBeginInfo();
            VK_CHECK(vkBeginCommandBuffer(cmdBuffer, &beginInfo));

            const VkClearValue clears { .color = { 0.1f, 0.1f, 0.1f, 1.0f } };
            const auto renderPassInfo = CreateRenderPassBeginInfo(
                context, 
                resources.ui_renderPass.renderPass, 
                resources.ui_renderPass.framebuffers[cmdBufferIdx],
                &clears
            );
            vkCmdBeginRenderPass (cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.ui_pipeline.pipeline);
            vkCmdPushConstants      (cmdBuffer, resources.ui_pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(resources.ui_pushConst), &resources.ui_pushConst);
            vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &resources.ui_vbo.vertexBuffer.buffer, &vboOffsets);
            vkCmdBindIndexBuffer    (cmdBuffer, resources.ui_vbo.indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
            
            FOR_ARRAY(vertexGroups, i)
            {
                uboOffsets = resources.ui_ibo.ALIGNMENT * i; 
                //! still abit unclear to me why we need to call twice (since first one could be called once) and also why both need dynamic
                vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.ui_pipeline.layout, 0, 1, &resources.ui_pipeline.sets[cmdBufferIdx], 1, &uboOffsets); 
                vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.ui_pipeline.layout, 1, 1, &resources.ui_pipeline.sets[cmdBufferIdx], 1, &uboOffsets); 
                vkCmdDrawIndexed        (cmdBuffer, vertexGroups[i].IndexCount(), 1, 0, vertexGroups[i].v1, 0);
            }
            
            //? wire mode
            //vkCmdBindPipeline (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.ui_pipeline_wire.pipeline);
            //FOR_ARRAY(vertexGroups, i) {
            //    vkCmdDrawIndexed(cmdBuffer, vertexGroups[i].IndexCount(), 1, 0, vertexGroups[i].v1, 0);
            //}

            vkCmdEndRenderPass(cmdBuffer);
            VK_CHECK(vkEndCommandBuffer(cmdBuffer));
        }


        inline void Render(const double dt, const app::Scene& scene)
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