//https://github.com/awwdev

#pragma once

#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Ctors.hpp"
#include "mini/Vulkan/Resources.hpp"
#include "mini/Resources/HostResources.hpp"
#include "mini/App/Scene.hpp"
#include "mini/RenderGraph/IRenderer.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/Box/String.hpp"
#include "mini/Window/AppEvents.hpp"

namespace mini::vk
{
    struct VkRenderer //: rendergraph::IRenderer
    {
        Context         context; //! must come first
        VkResources     resources;
        Commands        commands;
        Synchronization sync;

        uint32_t currentFrame = 0;
        
        VkRenderer(const vk::WindowHandle& wndHandle, hostRes::HostResources& hostResources)
            //: IRenderer { hostResources }
            : context   { wndHandle } //! must come first
            , resources { context }
        {
            //uniforms.alignment = context.physicalProps.limits.minUniformBufferOffsetAlignment;
            sync.Create(context);
            commands.Create(context);
            resources.Create(context, hostResources, commands);
        }


        void RecreateScwapchain()
        {
            vkDeviceWaitIdle(context.device);

            if (!context.RecreateSwapchain())
                return;

            /*
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
            */

            resources.text_renderPass.~RenderPass();
            CreateRenderPass_Text(context, resources.text_renderPass);

            resources.text_pipeline.~Pipeline();
            CreatePipeline_Text(
                context, 
                resources.text_pipeline,
                resources.text_shader, 
                resources.text_renderPass, 
                resources.text_ubo
            );

            commands.~Commands();
            commands.Create(context);
        }


        void UpdateVkResources(const app::Scene& scene, const double dt, rendergraph::RenderGraph& renderGraph)
        {
            resources.text_pushConst.wnd_w = wnd::window_w;
            resources.text_pushConst.wnd_h = wnd::window_h;

            rendergraph::UniformData_Text arr [4] = 
            {
                { { 32*1, 32*1, 0}, 0 },
                { { 32*2, 32*2, 0}, 1 },
                { { 32*3, 32*3, 0}, 2 },
                { { 32*4, 32*4, 0}, 3 },
            };
            resources.text_ubo.buffer.Store(arr, sizeof(rendergraph::UniformData_Text) * 4);

            //TODO: get texture array working

            //#extension GL_EXT_shader_16bit_storage : enable
            //VK_KHR_8bit_storage and VK_KHR_16bit_storage
            //VkPhysicalDeviceFeatures::shaderInt16

            //resources.ui_vbo.vertexBuffer.Store(vertices.Data(), vertices.Count()  * sizeof(utils::Vertex));
            //resources.ui_vbo.indexBuffer.Store (indices.Data(), indices.Count() * sizeof(uint32_t));
            //resources.ui_vbo.instanceBuffer.Store(instanceData.Data(), instanceData.Count() * sizeof(InstanceData_UI));
            //resources.ui_ibo.buffer.Store(uniforms.data, uniforms.CurrentSize());
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
                resources.text_renderPass.renderPass, 
                resources.text_renderPass.framebuffers[cmdBufferIdx],
                &clears
            );
            vkCmdBeginRenderPass (cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.text_pipeline.pipeline);
            vkCmdPushConstants      (cmdBuffer, resources.text_pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(resources.text_pushConst), &resources.text_pushConst);
            vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.text_pipeline.layout, 0, 1, &resources.text_pipeline.sets[cmdBufferIdx], 0, nullptr); 
            vkCmdDraw               (cmdBuffer, 6*4, 1, 0, 0);

            //vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &resources.ui_vbo.vertexBuffer.buffer, &vboOffsets);
            //vkCmdBindVertexBuffers  (cmdBuffer, 1, 1, &resources.ui_vbo.instanceBuffer.buffer, &vboOffsets);
            //vkCmdBindIndexBuffer    (cmdBuffer, resources.ui_vbo.indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
            
            //FOR_ARRAY(vertexGroups, i)
            {
                //uboOffsets = resources.ui_ibo.ALIGNMENT * i; 
                //! still abit unclear to me why we need to call twice (since first one could be called once) and also why both need dynamic
                //vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.ui_pipeline.layout, 0, 1, &resources.ui_pipeline.sets[cmdBufferIdx], 1, &uboOffsets); 
                //vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.ui_pipeline.layout, 1, 1, &resources.ui_pipeline.sets[cmdBufferIdx], 1, &uboOffsets); 
                //vkCmdDrawIndexed        (cmdBuffer, vertexGroups[i].IndexCount(), 3, 0, vertexGroups[i].v1, 0); 
            }
            
            //? wire mode
            //vkCmdBindPipeline (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.ui_pipeline_wire.pipeline);
            //FOR_ARRAY(vertexGroups, i) {
            //    vkCmdDrawIndexed(cmdBuffer, vertexGroups[i].IndexCount(), 1, 0, vertexGroups[i].v1, 0);
            //}

            vkCmdEndRenderPass(cmdBuffer);
            VK_CHECK(vkEndCommandBuffer(cmdBuffer));
        }


        inline void Render(const double dt, rendergraph::RenderGraph& renderGraph, app::Scene& scene)
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
            UpdateVkResources(scene, dt, renderGraph);
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