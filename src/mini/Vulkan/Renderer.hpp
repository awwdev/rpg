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
#include "mini/Window/WindowEvents.hpp"
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

        void UpdateVkResources_GameScene(const app::GameScene& scene, res::HostResources& hostRes, double dt)
        {
            const utils::Mat4f BIAS { 
                0.5, 0.0, 0.0, 0.0,
                0.0, 0.5, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.5, 0.5, 0.0, 1.0 
            };

            //resources.common_pushConsts.camera = scene.camera.GetOrthographic() * scene.sun.GetView();
            //resources.common_pushConsts.camera = scene.camera.GetPerspective()  * scene.playerController.GetView(scene.ecs);
            //resources.common_pushConsts.camera = scene.camera.GetPerspective()  * scene.camera.GetView();

            //TODO: this should be handled somewhere else which is active
            if (app::global::inputMode == app::global::PlayMode)
                resources.common_pushConsts.camera = resources.sky.pushConsts.camera = scene.playerController.camera.perspective * scene.playerController.camera.view;
            else
                resources.common_pushConsts.camera = resources.sky.pushConsts.camera = scene.editorController.camera.perspective * scene.editorController.camera.view;

            resources.common_pushConsts.sun    = scene.sun.GetOrthographic() * scene.sun.GetView(); //BIAS * 
            resources.common_pushConsts.sunDir = utils::Normalize(scene.sun.pos * 1);
            resources.ui.pushConsts.wnd_w = wnd::global::window_w;
            resources.ui.pushConsts.wnd_h = wnd::global::window_h;
            resources.sky.pushConsts.topColor = { 0.2f, 0.2f, 1.0f, 1 };
            resources.sky.pushConsts.botColor = { 1.0f, 1.0f, 1.0f, 1 };

            resources.ui.ubo.Clear();
            resources.ui.ubo.Store(scene.renderGraph.ui_ubo);

            resources.default.ubo.Clear();
            resources.default.ubo.Store(scene.renderGraph.default_ubo);

            //update terrain quadrant that is edited
            FOR_ARRAY(hostRes.terrain.editing.dirtyQuadrants, i){
                const auto quadrantIdx = hostRes.terrain.editing.dirtyQuadrants[i];
                resources.terrain.vbo.UpdateGroup(quadrantIdx, hostRes.terrain.GetQuadrant(quadrantIdx).verts);
            }
            
        }

        void RecordCommands(const uint32_t cmdBufferIdx, const double dt, const app::GameScene& scene)
        {
            //TODO: refactor into sub functions !!!

            auto& cmdBuffer = commands.cmdBuffers[cmdBufferIdx];
            VkDeviceSize vboOffsets { 0 };
            uint32_t     uboOffsets { 0 };

            auto beginInfo = vk::CreateCmdBeginInfo();
            VK_CHECK(vkBeginCommandBuffer(cmdBuffer, &beginInfo));

            const VkClearValue clears_sky [] { 
                { .color = { 0.1f, 0.1f, 0.7f, 1.0f } },
            };
            //const auto beginInfo_sky = resources.sky.renderPass.GetBeginInfo(cmdBufferIdx, ArrayCount(clears_sky), clears_sky);

            const VkClearValue clears_default [] { 
                { .color = { 0.0f, 0.0f, 0.0f, 0.0f } },
                { .depthStencil = { 0, 0 } }, //reversed z
                { .color = { 1.0f, 1.0f, 1.0f, 1.0f } },//ignored
            };
            const auto beginInfo_default = resources.default.renderPass.GetBeginInfo(
                cmdBufferIdx, ArrayCount(clears_default), clears_default);

            const VkClearValue clears_shadow [] { 
                { .depthStencil = { 0, 0 } } //reversed z
            };
            const auto beginInfo_shadow = resources.shadow.renderPass.GetBeginInfo(
                ArrayCount(clears_shadow), clears_shadow);

            const auto beginInfo_ui  = resources.ui.renderPass.GetBeginInfo(cmdBufferIdx);

            VkDeviceSize offsets = 0;

            vkCmdPushConstants(cmdBuffer, resources.terrain.pipelineShadow.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(resources.common_pushConsts), &resources.common_pushConsts);

            //! SHADOW MAP
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
                FOR_ARRAY(scene.renderGraph.default_ubo.groups.usedIndices, i)
                {
                    const auto vertOff   = resources.default.vbo.vertexGroups[i].begin;
                    const auto vertCount = resources.default.vbo.vertexGroups[i].count;
                    const auto instOff   = scene.renderGraph.default_ubo.groups.Get(i).begin;
                    const auto instCount = scene.renderGraph.default_ubo.groups.Get(i).count;
                    vkCmdDraw (cmdBuffer, vertCount, instCount, vertOff, instOff); 
                }
            }
            vkCmdEndRenderPass(cmdBuffer);

            //! DRAW 
            vkCmdBeginRenderPass(cmdBuffer, &beginInfo_default, VK_SUBPASS_CONTENTS_INLINE);
            {
                vkCmdPushConstants      (cmdBuffer, resources.sky.pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(resources.sky.pushConsts), &resources.sky.pushConsts);
                vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.sky.pipeline.pipeline);
                vkCmdDraw               (cmdBuffer, 14, 1, 0, 0); //!DOME VERTEX COUNT

                //! TERRAIN
                //TODO: culling (loops)
                vkCmdPushConstants      (cmdBuffer, resources.terrain.pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(resources.common_pushConsts), &resources.common_pushConsts);
                vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &resources.terrain.vbo.activeBuffer->buffer, &offsets);
                vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.terrain.pipeline.layout, 0, 1, &resources.terrain.pipeline.sets[cmdBufferIdx], 0, nullptr); 
                vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.terrain.pipeline.pipeline);
                vkCmdDraw               (cmdBuffer, resources.terrain.vbo.count, 1, 0, 0); 
                if (scene.editorController.terrainWireMode) {
                    vkCmdBindPipeline   (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.terrain.pipelineWire.pipeline);
                    vkCmdDraw           (cmdBuffer, resources.terrain.vbo.count, 1, 0, 0); 
                }

                //! DEFAULT
                vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.default.pipeline.layout, 0, 1, &resources.default.pipeline.sets[cmdBufferIdx], 0, nullptr); 
                vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.default.pipeline.pipeline);
                vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &resources.default.vbo.activeBuffer->buffer, &offsets);
                FOR_ARRAY(scene.renderGraph.default_ubo.groups.usedIndices, i)
                {
                    const auto vertOff   = resources.default.vbo.vertexGroups[i].begin;
                    const auto vertCount = resources.default.vbo.vertexGroups[i].count;
                    const auto instOff   = scene.renderGraph.default_ubo.groups.Get(i).begin;
                    const auto instCount = scene.renderGraph.default_ubo.groups.Get(i).count;
                    vkCmdDraw (cmdBuffer, vertCount, instCount, vertOff, instOff); 
                }
            }
            vkCmdEndRenderPass(cmdBuffer);
           
            //! UI
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


        void Render(const double dt, app::GameScene& scene, res::HostResources& hostRes)
        {
            if (wnd::global::events[wnd::Window_Resize] == wnd::Set){
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
            UpdateVkResources_GameScene(scene, hostRes, dt); //TODO: depends on scene
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
