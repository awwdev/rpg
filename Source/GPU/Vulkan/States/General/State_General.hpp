//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/States/General/General_RenderPass.hpp"
#include "GPU/Vulkan/States/General/General_Pipeline.hpp"
#include "GPU/Vulkan/States/General/General_Shader.hpp"
#include "GPU/Vulkan/States/General/General_Vertices.hpp"
#include "GPU/Vulkan/States/General/General_Uniforms.hpp"

#include "GPU/Vulkan/States/General/Wire/General_Wire_Shader.hpp"
#include "GPU/Vulkan/States/General/Wire/General_Wire_Pipeline.hpp"

#include "GPU/RenderData/RenderData.hpp"
#include "Resources/CpuResources.hpp"

namespace rpg::gpu::vuk {

struct State_General
{
    General_RenderPass  renderPass;
    General_Shader      shader;
    General_Pipeline    pipeline;
    General_Vertices    vertices;
    General_Uniforms    uniforms;

    General_Wire_Shader   wireShader;
    General_Wire_Pipeline wirePipeline;

    void Create(VkCommandPool cmdPool, Buffer& uboSun, Image& shadowMaps, res::CpuResources& cpuRes)
    {
        vertices    .Create(cmdPool, cpuRes);
        uniforms    .Create(uboSun, shadowMaps);
        shader      .Create();
        renderPass  .Create(cmdPool);
        pipeline    .Create(renderPass, shader, vertices, uniforms);

        wireShader  .Create();
        wirePipeline.Create(renderPass, wireShader, vertices, uniforms);
    }

    void Destroy()
    {
        pipeline    .Destroy();
        renderPass  .Destroy();
        shader      .Destroy();
        vertices    .Destroy();
        uniforms    .Destroy();

        wirePipeline.Destroy();
        wireShader  .Destroy();
    }

    void Update(gpu::RenderData& renderData, const res::CpuResources& cpuRes)
    {
        uniforms.Update(renderData.general.uboData_general_meta);
        vertices.Update(renderData, cpuRes);
    }

    void Record(VkCommandBuffer cmdBuffer)
    {
        vkCmdBeginRenderPass    (cmdBuffer, &renderPass.beginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &vertices.terrain.activeBuffer->buffer, vertices.offsets);
        vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.layout, 0, 
                                 uniforms.descriptors.descSets.count, uniforms.descriptors.descSets.data, 0, nullptr);
        //general
        vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);
        vkCmdDraw               (cmdBuffer, vertices.terrain.count, 1, 0, 0);
        //wire
        vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, wirePipeline.pipeline);
        vkCmdDraw               (cmdBuffer, vertices.terrain.count, 1, 0, 0);
        //Objects
        vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);
        vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &vertices.objects.activeBuffer->buffer, vertices.offsets);
        vkCmdDraw               (cmdBuffer, vertices.objects.count, 1, 0, 0);

        vkCmdEndRenderPass      (cmdBuffer);
    };
    
};

}//NS