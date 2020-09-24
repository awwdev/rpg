//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/States/General/General_RenderPass.hpp"
#include "gpu/Vulkan/States/General/General_Pipeline.hpp"
#include "gpu/Vulkan/States/General/General_Shader.hpp"
#include "gpu/Vulkan/States/General/General_Vertices.hpp"
#include "gpu/Vulkan/States/General/General_Uniforms.hpp"

#include "gpu/Vulkan/States/General/Terrain/Terrain_Shader.hpp"
#include "gpu/Vulkan/States/General/Terrain/Terrain_Pipeline.hpp"
#include "gpu/Vulkan/States/General/Terrain/Terrain_Wire_Shader.hpp"
#include "gpu/Vulkan/States/General/Terrain/Terrain_Wire_Pipeline.hpp"

#include "gpu/RenderData/RenderData.hpp"
#include "res/_Old/CpuResources.hpp"
#include "res/Resources.hpp"

namespace rpg::gpu::vuk {

struct State_General
{
    General_RenderPass  renderPass;
    General_Shader      shader;
    General_Pipeline    pipeline;
    General_Vertices    vertices;
    General_Uniforms    uniforms;

    Terrain_Shader        terrainShader;
    Terrain_Pipeline      terrainPipeline;
    Terrain_Wire_Shader   terrainWireShader;
    Terrain_Wire_Pipeline terrainWirePipeline;

    void Create(VkCommandPool cmdPool, Buffer& uboSun, Image& shadowMaps, res::CpuResources& cpuRes, res::Resources& res)
    {
        vertices    .Create(cmdPool, res.models);
        uniforms    .Create(uboSun, shadowMaps);
        shader      .Create();
        renderPass  .Create(cmdPool);
        pipeline    .Create(renderPass, shader, vertices, uniforms);

        terrainShader       .Create();
        terrainPipeline     .Create(renderPass, terrainShader, vertices, uniforms);
        terrainWireShader   .Create();
        terrainWirePipeline .Create(renderPass, terrainWireShader, vertices, uniforms);
    }

    void Destroy()
    {
        pipeline    .Destroy();
        renderPass  .Destroy();
        shader      .Destroy();
        vertices    .Destroy();
        uniforms    .Destroy();

        terrainPipeline     .Destroy();
        terrainShader       .Destroy();
        terrainWirePipeline .Destroy();
        terrainWireShader   .Destroy();
    }

    void Update(gpu::RenderData& renderData, const res::CpuResources& cpuRes)
    {
        uniforms.Update(renderData.general);
        vertices.Update(renderData, cpuRes);
    }

    void Record(VkCommandBuffer cmdBuffer, RenderData_General& rdGeneral)
    {
        vkCmdBeginRenderPass    (cmdBuffer, &renderPass.beginInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.layout, 0, 
                                 uniforms.descriptors.descSets.count, uniforms.descriptors.descSets.data, 0, nullptr);

        //terrain
        vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &vertices.terrain.activeBuffer->buffer, vertices.offsets);
        vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, terrainPipeline.pipeline);
        vkCmdDraw               (cmdBuffer, vertices.terrain.count, 1, 0, 0);
        //terrain wire
        vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, terrainWirePipeline.pipeline);
        vkCmdDraw               (cmdBuffer, vertices.terrain.count, 1, 0, 0);

        //objects 
        vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);
        vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &vertices.objects.activeBuffer->buffer, vertices.offsets);

        for(uint32_t modelType = 0; modelType < (uint32_t) res::ModelType::ENUM_END; ++modelType)
        {
            const auto& vertexRange = rdGeneral.vertexRanges[modelType];
            const auto& sboModels   = rdGeneral.sboModels[modelType];
            if (!sboModels.Empty()) {
                //vkCmdPushConstant //for indexing into sbo
                vkCmdDraw(cmdBuffer, vertexRange.count, sboModels.Count(), vertexRange.beginIdx, 0);
            }
        }

        vkCmdEndRenderPass (cmdBuffer);
    };
    
};

}//NS