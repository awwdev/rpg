//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Ctors.hpp"
#include "mini/Vulkan/Resources.hpp"
#include "mini/Resources/HostResources.hpp"
#include "mini/App/Scene.hpp"
#include "mini/RenderGraph/RenderGraph.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/Box/String.hpp"
#include "mini/Window/AppEvents.hpp"
#include "mini/Utils/DeltaTime.hpp"

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

            //? same as when creating stuff but with dtor
            resources.text_renderPass.~RenderPass();
            CreateRenderPass_Text(context, resources.text_renderPass);

            resources.text_pipeline.~Pipeline();
            CreatePipeline_Text(
                context, 
                resources.text_pipeline,
                resources.text_shader, 
                resources.text_renderPass, 
                resources.text_ubo_array
            );

            commands.~Commands();
            commands.Create(context);

            //TODO: find a way to keep track what was created and then recreate it
            //maybe a unified method Create() that also can be used OnRecreate (need of checks)
        }


        void UpdateVkResources(const app::Scene& scene, const double dt)
        {
            resources.text_pushConst.wnd_w = wnd::window_w;
            resources.text_pushConst.wnd_h = wnd::window_h;

            resources.text_ubo_array.Store(rendergraph::renderGraph.uboText);
        }

        void RecordCommands(const uint32_t cmdBufferIdx, const double dt, const app::Scene& scene)
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

            //? TEXT
            vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.text_pipeline.pipeline);
            vkCmdPushConstants      (cmdBuffer, resources.text_pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(resources.text_pushConst), &resources.text_pushConst);
            vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.text_pipeline.layout, 0, 1, &resources.text_pipeline.sets[cmdBufferIdx], 0, nullptr); 
            vkCmdDraw               (cmdBuffer, resources.text_ubo_array.count * 6, 1, 0, 0); 

            vkCmdEndRenderPass(cmdBuffer);
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




/*
dynamic ubo binding 
//FOR_ARRAY(vertexGroups, i)
{
    //uboOffsets = resources.ui_ibo.ALIGNMENT * i; 
    //! still abit unclear to me why we need to call twice (since first one could be called once) and also why both need dynamic
    //vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.ui_pipeline.layout, 0, 1, &resources.ui_pipeline.sets[cmdBufferIdx], 1, &uboOffsets); 
    //vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.ui_pipeline.layout, 1, 1, &resources.ui_pipeline.sets[cmdBufferIdx], 1, &uboOffsets); 
    //vkCmdDrawIndexed        (cmdBuffer, vertexGroups[i].IndexCount(), 3, 0, vertexGroups[i].v1, 0); 
}
*/