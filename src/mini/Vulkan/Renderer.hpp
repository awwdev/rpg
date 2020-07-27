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
            resources.Create(hostResources, commands.cmdPool);
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

        //TODO: depends on scene
        void UpdateVkResources_GameScene(const app::GameScene& scene, const double dt)
        {
            const math::Mat4f BIAS { 
                0.5, 0.0, 0.0, 0.0,
                0.0, 0.5, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.5, 0.5, 0.0, 1.0 
            };

            //resources.common_pushConsts.camera = scene.camera.GetOrthographic() * scene.sun.GetView();
            resources.common_pushConsts.camera = scene.camera.GetPerspective()  * scene.camera.GetView();
            resources.common_pushConsts.sun    = scene.camera.GetOrthographic() * scene.sun.GetView(); //BIAS * 
            resources.common_pushConsts.sunDir = math::Normalize(scene.sun.pos);
            resources.ui.pushConsts.wnd_w = wnd::window_w;
            resources.ui.pushConsts.wnd_h = wnd::window_h;

            resources.ui.ubo.Clear();
            resources.ui.ubo.Store(scene.renderGraph.ui_ubo);

            resources.default.ubo.Clear();
            resources.default.ubo.Store(scene.renderGraph.default_ubo);

            //TODO: selectively update terrain data
            resources.terrain.vbo.Clear();
            FOR_CARRAY(scene.terrain.quadrants, i) {
                resources.terrain.vbo.AppendGroup(scene.terrain.quadrants[i].verts);
            }
        }

        void RecordCommands(const uint32_t cmdBufferIdx, const double dt, const app::GameScene& scene)
        {
            auto& cmdBuffer = commands.cmdBuffers[cmdBufferIdx];
            VkDeviceSize vboOffsets { 0 };
            uint32_t     uboOffsets { 0 };

            auto beginInfo = vk::CreateCmdBeginInfo();
            VK_CHECK(vkBeginCommandBuffer(cmdBuffer, &beginInfo));

            const auto beginInfo_ui = resources.ui.renderPass.GetBeginInfo(cmdBufferIdx);

            const VkClearValue clears_default [] { 
                { .color = { 0.1f, 0.1f, 0.1f, 1.0f } },
                { .depthStencil = { 0, 0 } } //reversed z
            };
            const auto beginInfo_default = resources.default.renderPass.GetBeginInfo(
                cmdBufferIdx, ARRAY_COUNT(clears_default), clears_default);

            const VkClearValue clears_shadow [] { 
                { .depthStencil = { 0, 0 } } //reversed z
            };
            const auto beginInfo_shadow = resources.shadow.renderPass.GetBeginInfo(
                ARRAY_COUNT(clears_shadow), clears_shadow);


            VkDeviceSize offsets = 0;

            vkCmdPushConstants(cmdBuffer, resources.terrain.pipelineShadow.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(resources.common_pushConsts), &resources.common_pushConsts);

            //TODO: FIX RESIZE ERROR NOW!

            //! SHADOW
            vkCmdBeginRenderPass(cmdBuffer, &beginInfo_shadow, VK_SUBPASS_CONTENTS_INLINE);
            {
                //TERRAIN
                vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &resources.terrain.vbo.activeBuffer->buffer, &offsets);
                vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.terrain.pipelineShadow.pipeline);
                vkCmdDraw               (cmdBuffer, resources.terrain.vbo.count, 1, 0, 0); 

                //DEFAULT
                vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.default.pipelineShadow.layout, 0, 1, &resources.default.pipelineShadow.sets[cmdBufferIdx], 0, nullptr); 
                vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.default.pipelineShadow.pipeline);
                vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &resources.default.vbo.activeBuffer->buffer, &offsets);
                FOR_USED_INDICES_MAP_BEGIN(scene.renderGraph.default_ubo.groups, usedIndex)
                {
                    const auto vertOff   = resources.default.vbo.vertexGroups[usedIndex].begin;
                    const auto vertCount = resources.default.vbo.vertexGroups[usedIndex].count;
                    const auto instOff   = scene.renderGraph.default_ubo.groups.Get(usedIndex).begin;
                    const auto instCount = scene.renderGraph.default_ubo.groups.Get(usedIndex).count;
                    vkCmdDraw (cmdBuffer, vertCount, instCount, vertOff, instOff); 
                }
                FOR_USED_INDICES_MAP_END
            }
            vkCmdEndRenderPass(cmdBuffer);

            vkCmdBeginRenderPass(cmdBuffer, &beginInfo_default, VK_SUBPASS_CONTENTS_INLINE);
            {
                //! TERRAIN
                //TODO: culling (loops)
                vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &resources.terrain.vbo.activeBuffer->buffer, &offsets);
                vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.terrain.pipeline.layout, 0, 1, &resources.terrain.pipeline.sets[cmdBufferIdx], 0, nullptr); 
                vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.terrain.pipeline.pipeline);
                vkCmdDraw               (cmdBuffer, resources.terrain.vbo.count, 1, 0, 0); 
                vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.terrain.pipelineWire.pipeline);
                vkCmdDraw               (cmdBuffer, resources.terrain.vbo.count, 1, 0, 0); 

                //! DEFAULT
                vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.default.pipeline.layout, 0, 1, &resources.default.pipeline.sets[cmdBufferIdx], 0, nullptr); 
                vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.default.pipeline.pipeline);
                vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &resources.default.vbo.activeBuffer->buffer, &offsets);
                FOR_USED_INDICES_MAP_BEGIN(scene.renderGraph.default_ubo.groups, usedIndex)
                {
                    const auto vertOff   = resources.default.vbo.vertexGroups[usedIndex].begin;
                    const auto vertCount = resources.default.vbo.vertexGroups[usedIndex].count;
                    const auto instOff   = scene.renderGraph.default_ubo.groups.Get(usedIndex).begin;
                    const auto instCount = scene.renderGraph.default_ubo.groups.Get(usedIndex).count;
                    vkCmdDraw (cmdBuffer, vertCount, instCount, vertOff, instOff); 
                }
                FOR_USED_INDICES_MAP_END
            }
            vkCmdEndRenderPass(cmdBuffer);
           
            //! TEXT
            vkCmdBeginRenderPass(cmdBuffer, &beginInfo_ui, VK_SUBPASS_CONTENTS_INLINE);
            {
                vkCmdPushConstants      (cmdBuffer, resources.ui.pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(resources.ui.pushConsts), &resources.ui.pushConsts);
                vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.ui.pipeline.pipeline);
                vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.ui.pipeline.layout, 0, 1, &resources.ui.pipeline.sets[cmdBufferIdx], 0, nullptr); 
                vkCmdDraw               (cmdBuffer, resources.ui.ubo.count * 6, 1, 0, 0); 
            }
            vkCmdEndRenderPass(cmdBuffer);

            VK_CHECK(vkEndCommandBuffer(cmdBuffer));
        }


        void Render(const double dt, app::GameScene& scene)
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
            UpdateVkResources_GameScene(scene, dt); //TODO: depends on scene
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
