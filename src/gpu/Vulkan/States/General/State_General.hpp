//https://github.com/awwdev

#pragma once
#include "gpu/RenderData/RenderData.hpp"
#include "res/Resources.hpp"

//general
#include "gpu/Vulkan/States/General/General_RenderPass.hpp"
#include "gpu/Vulkan/States/General/General_Pipeline.hpp"
#include "gpu/Vulkan/States/General/General_Shader.hpp"
#include "gpu/Vulkan/States/General/General_Vertices.hpp"
#include "gpu/Vulkan/States/General/General_Uniforms.hpp"
//terrain
#include "gpu/Vulkan/States/General/Terrain/Terrain_Shader.hpp"
#include "gpu/Vulkan/States/General/Terrain/Terrain_Pipeline.hpp"
#include "gpu/Vulkan/States/General/Terrain/TerrainWire_Shader.hpp"
#include "gpu/Vulkan/States/General/Terrain/TerrainWire_Pipeline.hpp"
//instances


namespace rpg::gpu::vuk {

struct State_General
{
    //general
    General_RenderPass generalRenderPass;
    General_Vertices generalVertices;
    General_Uniforms generalUniforms;
    //terrain
    Terrain_Shader terrainShader;
    Terrain_Pipeline terrainPipeline;
    TerrainWire_Shader terrainWireShader;
    TerrainWire_Pipeline terrainWirePipeline;

    void Create(VkCommandPool cmdPool, Buffer& uboSun, Image& shadowMaps, res::Resources& res)
    {
        //general
        generalVertices     .Create(cmdPool, res.meshes, res.terrain);
        generalUniforms     .Create(cmdPool, uboSun, shadowMaps, res.textures);
        generalRenderPass   .Create(cmdPool);
        //terrain
        terrainShader       .Create();
        terrainPipeline     .Create(generalRenderPass, terrainShader, generalVertices, generalUniforms);
        terrainWireShader   .Create();
        terrainWirePipeline .Create(generalRenderPass, terrainWireShader, generalVertices, generalUniforms);
        //instances
    }

    void Destroy()
    {
        //general
        generalRenderPass.Destroy();
        generalVertices.Destroy();
        generalUniforms.Destroy();
        //terrain
        terrainShader.Destroy();      
        terrainPipeline.Destroy();     
        terrainWireShader.Destroy();      
        terrainWirePipeline.Destroy();  
        //instances  
    }

    void Update(gpu::RenderData& renderData, const res::Resources& resources)
    {
        generalUniforms.Update(renderData.general, resources.terrain);
        generalVertices.Update(renderData.general, resources.terrain);
    }

    void RecordTerrain(VkCommandBuffer cmdBuffer, RenderData_General& render_data_general)
    {
        vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, terrainPipeline.pipeline);
        vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, terrainPipeline.layout, 0, 
        generalUniforms.descriptors.descSets.count, generalUniforms.descriptors.descSets.data, 0, nullptr);

        vkCmdBindIndexBuffer    (cmdBuffer, generalVertices.iboTerrain.activeBuffer->buffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &generalVertices.vboTerrain.activeBuffer->buffer, generalVertices.offsets);
        vkCmdDrawIndexed        (cmdBuffer, generalVertices.iboTerrain.count, 1, 0, 0 , 0);

        if (render_data_general.enableTerrainWire)
        {
            vkCmdBindPipeline   (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, terrainWirePipeline.pipeline);
            vkCmdDrawIndexed    (cmdBuffer, generalVertices.iboTerrain.count, 1, 0, 0 , 0);
        }
    }

    void RecordInstances(VkCommandBuffer cmdBuffer, RenderData_General& render_data_general)
    {
        vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &generalVertices.vboMeshes.activeBuffer->buffer, generalVertices.offsets);
        vkCmdBindIndexBuffer    (cmdBuffer, generalVertices.iboMeshes.activeBuffer->buffer, 0, VkIndexType::VK_INDEX_TYPE_UINT32);

        auto const& material_array = render_data_general.instance_datas; //multiple arrays
        FOR_C_ARRAY(material_array, matIdx)
        {
            auto const& mesh_array = material_array[matIdx];
            //TODO: bind UBO and pipeline according to material
            //vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, generalPipeline.layout, 0, 
            //generalUniforms.descriptors.descSets.count, generalUniforms.descriptors.descSets.data, 0, nullptr);
            //vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, generalPipeline.pipeline);

            FOR_C_ARRAY(mesh_array, meshIdx)
            {
                auto const& inst_array  = mesh_array[meshIdx];
                if (inst_array.Empty()) continue;

                auto const& vertexRange = generalVertices.vboMeshesVertexRanges[meshIdx];
                auto const& indexRange  = generalVertices.iboMeshesIndexRanges [meshIdx];
                //vkCmdDrawIndexed(cmdBuffer, indexRange.count, inst_array.Count(), indexRange.index, vertexRange.index, 0); //instance idx? buffer offsets?
                dbg::LogInfo(inst_array.Count());
            }
        }
    }

    void Record(VkCommandBuffer cmdBuffer, RenderData_General& render_data_general)
    {
        vkCmdBeginRenderPass(cmdBuffer, &generalRenderPass.beginInfo, VK_SUBPASS_CONTENTS_INLINE);
        //TODO: 0 set for camera 1 set for specific stuff (but currently all inside set 0)
        RecordTerrain(cmdBuffer, render_data_general);
        //RecordInstances(cmdBuffer, render_data_general);
        vkCmdEndRenderPass(cmdBuffer);
    };
    
};

}//ns