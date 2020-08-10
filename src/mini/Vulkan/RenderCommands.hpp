//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Resources.hpp"
#include "mini/App/Scene.hpp"
#include "mini/Resources/HostResources.hpp"


namespace mini::vk {

inline void RecordCommands(
    VkResources& resources,
    Commands& commands, 
    const uint32_t cmdBufferIdx, 
    const double dt, 
    const app::GameScene& scene)
{
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
        //? SKYDOME
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

} //ns