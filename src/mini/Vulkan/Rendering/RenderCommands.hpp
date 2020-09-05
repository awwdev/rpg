//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Resources/Resources.hpp"
#include "mini/App/Scene.hpp"
#include "mini/Resources/HostResources.hpp"

//vkCmdSetDepthBias(
//    cmdBuffer, 
//    scene.renderGraph.depthBiasConstantFactor,
//    scene.renderGraph.depthBiasClamp,
//    scene.renderGraph.depthBiasSlopeFactor
//);

namespace mini::vk {

inline void ShadowMap(VkCommandBuffer cmdBuffer, const uint32_t cmdBufferIdx, VkResources& resources, const app::GameScene& scene)
{
    VkDeviceSize offsets {};
    const VkClearValue clears_shadow [] { 
        { .depthStencil = { 0, 0 } } //reversed z        
    };

    auto& renderPassShadow = resources.shadow.renderPass;
    auto& pipelineTerrain  = resources.terrain.pipelineShadow;
    auto& pushConsts       = resources.shadow.pushConsts;

    for(uint32_t cascadeIdx = 0; cascadeIdx < renderPassShadow.layerCount; ++cascadeIdx)
    {
        const auto beginInfo = renderPassShadow.GetBeginInfo(cascadeIdx, ArrayCount(clears_shadow), clears_shadow);
        vkCmdBeginRenderPass    (cmdBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineTerrain.pipeline);
        pushConsts.cascade = cascadeIdx;
        vkCmdPushConstants      (cmdBuffer, pipelineTerrain.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(pushConsts), &pushConsts);
        vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &resources.terrain.vbo.activeBuffer->buffer, &offsets);
        vkCmdDraw               (cmdBuffer, resources.terrain.vbo.count, 1, 0, 0); 
        vkCmdEndRenderPass      (cmdBuffer);
    }

    //vkCmdSetDepthBias(
    //    cmdBuffer, 
    //    scene.renderGraph.depthBiasConstantFactor,
    //    scene.renderGraph.depthBiasClamp,
    //    scene.renderGraph.depthBiasSlopeFactor
    //);

    //DEFAULT
    //vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.default.pipelineShadow.layout, 0, 1, &resources.default.pipelineShadow.sets[cmdBufferIdx], 0, nullptr); 
    //vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &resources.default.vbo.activeBuffer->buffer, &offsets);
    //vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.default.pipelineShadow.pipeline);
    //FOR_ARRAY(scene.renderGraph.default_ubo.groupsVertexColor.usedIndices, i)
    //{
    //    const auto& ubo = scene.renderGraph.default_ubo.groupsVertexColor;
    //    const auto idx  = ubo.usedIndices[i];
    //    const auto vertOff   = resources.default.vbo.vertexGroups[idx].begin;
    //    const auto vertCount = resources.default.vbo.vertexGroups[idx].count;
    //    const auto instOff   = ubo.Get(idx).begin;
    //    const auto instCount = ubo.Get(idx).count;
    //    vkCmdDraw (cmdBuffer, vertCount, instCount, vertOff, instOff); 
    //}
}

inline void Geometry(VkCommandBuffer cmdBuffer, const uint32_t cmdBufferIdx, VkResources& resources, const app::GameScene& scene)
{
    const VkClearValue clears_default [] { 
        { .color = { 0.0f, 0.0f, 0.0f, 0.0f } },
        { .depthStencil = { 0, 0 } }, //reversed z
        { .color = { 1.0f, 1.0f, 1.0f, 1.0f } },//ignored
    };

    const auto beginInfo_default = resources.default.renderPass.GetBeginInfo(cmdBufferIdx, ArrayCount(clears_default), clears_default);
    VkDeviceSize offsets {};

    vkCmdBeginRenderPass(cmdBuffer, &beginInfo_default, VK_SUBPASS_CONTENTS_INLINE);

    //?SKYDOME
    //vkCmdPushConstants      (cmdBuffer, resources.sky.pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(resources.sky.pushConsts), &resources.sky.pushConsts);
    //vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.sky.pipeline.pipeline);
    //vkCmdDraw               (cmdBuffer, 14, 1, 0, 0);

    //?TERRAIN
    //TODO: culling (loops)
    vkCmdPushConstants      (cmdBuffer, resources.terrain.pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(resources.common_pushConsts2), &resources.common_pushConsts2);
    vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &resources.terrain.vbo.activeBuffer->buffer, &offsets);
    vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.terrain.pipeline.layout, 0, 1, &resources.terrain.pipeline.sets[cmdBufferIdx], 0, nullptr); 
    vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.terrain.pipeline.pipeline);
    vkCmdDraw               (cmdBuffer, resources.terrain.vbo.count, 1, 0, 0); 
    if (scene.editorController.terrainWireMode) {
        vkCmdBindPipeline   (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.terrain.pipelineWire.pipeline);
        vkCmdDraw           (cmdBuffer, resources.terrain.vbo.count, 1, 0, 0); 
    }

    //?DEFAULT
    /*vkCmdPushConstants      (cmdBuffer, resources.terrain.pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(resources.common_pushConsts), &resources.common_pushConsts);
    vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.default.pipelineVertexColor.layout, 0, 1, &resources.default.pipelineVertexColor.sets[cmdBufferIdx], 0, nullptr); 
    vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &resources.default.vbo.activeBuffer->buffer, &offsets);
    vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.default.pipelineVertexColor.pipeline);
    
    FOR_ARRAY(scene.renderGraph.default_ubo.groupsVertexColor.usedIndices, i)
    {
        const auto& ubo = scene.renderGraph.default_ubo.groupsVertexColor;
        const auto idx  = ubo.usedIndices[i];
        const auto vertOff   = resources.default.vbo.vertexGroups[idx].begin;
        const auto vertCount = resources.default.vbo.vertexGroups[idx].count;
        const auto instOff   = ubo.Get(idx).begin;
        const auto instCount = ubo.Get(idx).count;
        vkCmdDraw (cmdBuffer, vertCount, instCount, vertOff, instOff); 
    }*/

    /*
    vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.default.pipelineTexture.layout, 0, 1, &resources.default.pipelineTexture.sets[cmdBufferIdx], 0, nullptr); 
    vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.default.pipelineTexture.pipeline);
    vkCmdPushConstants(cmdBuffer,resources.default.pipelineTexture.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(resources.common_pushConsts), &resources.common_pushConsts);
    FOR_ARRAY(scene.renderGraph.default_ubo.groupsTexture.usedIndices, i)
    {
        const auto& ubo = scene.renderGraph.default_ubo.groupsTexture;
        const auto idx  = ubo.usedIndices[i];
        const auto vertOff   = resources.default.vbo.vertexGroups[idx].begin;
        const auto vertCount = resources.default.vbo.vertexGroups[idx].count;
        const auto instOff   = ubo.Get(idx).begin;
        const auto instCount = ubo.Get(idx).count;
        vkCmdDraw (cmdBuffer, vertCount, instCount, vertOff, instOff); 
    }
    */

    vkCmdEndRenderPass(cmdBuffer);
}

inline void UI(VkCommandBuffer cmdBuffer, const uint32_t cmdBufferIdx, VkResources& resources, const app::GameScene& scene)
{
    const auto beginInfo_ui = resources.ui.renderPass.GetBeginInfo(cmdBufferIdx);
    vkCmdBeginRenderPass(cmdBuffer, &beginInfo_ui, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdPushConstants      (cmdBuffer, resources.ui.pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(resources.ui.pushConsts), &resources.ui.pushConsts);
    vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.ui.pipeline.pipeline);
    vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resources.ui.pipeline.layout, 0, 1, &resources.ui.pipeline.sets[cmdBufferIdx], 0, nullptr); 
    
    if (resources.ui.ubo.count > 0)
        vkCmdDraw(cmdBuffer, resources.ui.ubo.count * 6, 1, 0, 0); 

    vkCmdEndRenderPass(cmdBuffer);
}

inline void RecordCommands(
    VkResources& resources,
    Commands& commands, 
    const uint32_t cmdBufferIdx, 
    const double dt, 
    const app::GameScene& scene)
{
    auto cmdBuffer = commands.cmdBuffers[cmdBufferIdx];
    const auto beginInfo = vk::CreateCmdBeginInfo();
    VkCheck(vkBeginCommandBuffer(cmdBuffer, &beginInfo));

    ShadowMap   (cmdBuffer, cmdBufferIdx, resources, scene);
    Geometry    (cmdBuffer, cmdBufferIdx, resources, scene);
    UI          (cmdBuffer, cmdBufferIdx, resources, scene); 

    VkCheck(vkEndCommandBuffer(cmdBuffer));
}

} //ns