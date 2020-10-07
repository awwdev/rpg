//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/States/General/General_RenderPass.hpp"
#include "gpu/Vulkan/States/General/General_Pipeline.hpp"
#include "gpu/Vulkan/States/General/General_Shader.hpp"
#include "gpu/Vulkan/States/General/General_Vertices.hpp"
#include "gpu/Vulkan/States/General/General_Uniforms.hpp"

#include "gpu/Vulkan/States/General/Terrain/State_Terrain.hpp"
#include "gpu/Vulkan/States/General/Foliage/State_Foliage.hpp"

#include "gpu/RenderData/RenderData.hpp"
#include "res/Resources.hpp"

namespace rpg::gpu::vuk {

struct State_General
{
    General_RenderPass    generalRenderPass;
    General_Vertices      generalVertices;
    General_Uniforms      generalUniforms;
    General_Shader        generalShader;
    General_Pipeline      generalPipeline;

    State_Terrain         terrain;
    State_Foliage         foliage;

    void Create(VkCommandPool cmdPool, Buffer& uboSun, Image& shadowMaps, res::Resources& res)
    {
        generalVertices     .Create(cmdPool, res.meshes);
        generalUniforms     .Create(cmdPool, uboSun, shadowMaps, res.textures);
        generalRenderPass   .Create(cmdPool);
        generalShader       .Create();
        generalPipeline     .Create(generalRenderPass, generalShader, generalVertices, generalUniforms);

        terrain             .Create(generalRenderPass, generalVertices, generalUniforms);
        foliage             .Create(generalRenderPass, generalVertices, generalUniforms);
    }

    void Destroy()
    {
        generalPipeline     .Destroy();
        generalRenderPass   .Destroy();
        generalShader       .Destroy();
        generalVertices     .Destroy();
        generalUniforms     .Destroy();

        terrain             .Destroy();
        foliage             .Destroy();
    }

    void Update(gpu::RenderData& renderData, const res::Resources& resources)
    {
        generalUniforms.Update(renderData.general);
        generalVertices.Update(renderData.general, resources.terrain);
    }

    void Record(VkCommandBuffer cmdBuffer, RenderData_General& rdGeneral)
    {
        vkCmdBeginRenderPass    (cmdBuffer, &generalRenderPass.beginInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, generalPipeline.layout, 0, 
                                 generalUniforms.descriptors.descSets.count, generalUniforms.descriptors.descSets.data, 0, nullptr);

        //?terrain
        vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &generalVertices.vboTerrain.activeBuffer->buffer, generalVertices.offsets);
        vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, terrain.pipeline.pipeline);
        vkCmdDraw               (cmdBuffer, generalVertices.vboTerrain.count, 1, 0, 0);
        rdGeneral.dbgVertCountTerrain += generalVertices.vboTerrain.count;

        //?terrain wire
        if (rdGeneral.enableTerrainWire)
        {
            vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, terrain.wirePipeline.pipeline);
            vkCmdDraw(cmdBuffer, generalVertices.vboTerrain.count, 1, 0, 0);
            rdGeneral.dbgVertCountTerrain += generalVertices.vboTerrain.count;
        }

        //?meshes 
        vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, generalPipeline.pipeline);
        vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &generalVertices.vboMeshes.activeBuffer->buffer, generalVertices.offsets);

        uint32_t instanceIdx = 0;
        for(idx_t meshIdx = (idx_t) res::MeshEnum::None + 1; meshIdx < (idx_t) res::MeshEnum::ENUM_END; ++meshIdx)
        {
            auto const& meshInstances = rdGeneral.meshInstances[meshIdx];
            if (meshInstances.Empty()) 
                continue;

            dbg::Assert(res::MESH_MATERIAL_GROUPS.Contains(meshIdx), "mesh material mapping missing");
            auto const meshMaterial = res::MESH_MATERIAL_GROUPS.Get(meshIdx);

            switch(meshMaterial) //prev material compare, to avoid bind same pipeline?
            {
                case res::MeshMaterialEnum::Foliage: 
                    vkCmdBindPipeline (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, foliage.pipeline.pipeline);
                break;

                case res::MeshMaterialEnum::Metal: 
                case res::MeshMaterialEnum::Default: 
                    vkCmdBindPipeline (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, generalPipeline.pipeline);
                break;

                default: dbg::Assert(false, "mesh material pipeline missing");
            }

            auto const& vertexRange = generalVertices.vboMeshesVertexRanges[meshIdx];
            vkCmdDraw(cmdBuffer, vertexRange.count, meshInstances.Count(), vertexRange.index, instanceIdx);
            instanceIdx += meshInstances.Count();

            rdGeneral.dbgVertCountInstanced += vertexRange.count * meshInstances.Count();
        }

        vkCmdEndRenderPass(cmdBuffer);
    };
    
};

}//NS