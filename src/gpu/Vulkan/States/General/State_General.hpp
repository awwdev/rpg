//https://github.com/awwdev

#pragma once
//general
#include "gpu/Vulkan/States/General/General_RenderPass.hpp"
#include "gpu/Vulkan/States/General/General_Pipeline.hpp"
#include "gpu/Vulkan/States/General/General_Shader.hpp"
#include "gpu/Vulkan/States/General/General_Vertices.hpp"
#include "gpu/Vulkan/States/General/General_Uniforms.hpp"
//debug
#include "gpu/Vulkan/States/General/Debug/Simple_Shader.hpp"
#include "gpu/Vulkan/States/General/Debug/Wire_Pipeline.hpp"
#include "gpu/Vulkan/States/General/Debug/Lines_Pipeline.hpp"
//terrain
#include "gpu/Vulkan/States/General/Terrain/Terrain_Shader.hpp"
#include "gpu/Vulkan/States/General/Terrain/Terrain_Pipeline.hpp"
//foliage
#include "gpu/Vulkan/States/General/Foliage/Foliage_Shader.hpp"
#include "gpu/Vulkan/States/General/Foliage/Foliage_Pipeline.hpp"

#include "gpu/RenderData/RenderData.hpp"
#include "res/Resources.hpp"

namespace rpg::gpu::vuk {

struct State_General
{
    //general
    General_RenderPass generalRenderPass;
    General_Vertices generalVertices;
    General_Uniforms generalUniforms;
    General_Shader generalShader;
    General_Pipeline generalPipeline;
    //debug
    Simple_Shader simpleShader;
    Wire_Pipeline wirePipeline;
    Lines_Pipeline linesPipeline;
    //terrain
    Terrain_Shader terrainShader;
    Terrain_Pipeline terrainPipeline;
    //foliage
    Foliage_Shader foliageShader;
    Foliage_Pipeline foliagePipeline;

    void Create(VkCommandPool cmdPool, Buffer& uboSun, Image& shadowMaps, res::Resources& res)
    {
        generalVertices     .Create(cmdPool, res.meshes, res.terrain);
        generalUniforms     .Create(cmdPool, uboSun, shadowMaps, res.textures);
        generalRenderPass   .Create(cmdPool);
        generalShader       .Create();
        generalPipeline     .Create(generalRenderPass, generalShader, generalVertices, generalUniforms);
        //debug
        simpleShader        .Create();
        wirePipeline        .Create(generalRenderPass, simpleShader, generalVertices, generalUniforms);
        linesPipeline       .Create(generalRenderPass, generalShader, generalVertices, generalUniforms);
        //terrain
        terrainShader       .Create();
        terrainPipeline     .Create(generalRenderPass, terrainShader, generalVertices, generalUniforms);
        //foliage
        foliageShader       .Create();
        foliagePipeline     .Create(generalRenderPass, foliageShader, generalVertices, generalUniforms);
    }

    void Destroy()
    {
        //general
        generalPipeline.Destroy();
        generalRenderPass.Destroy();
        generalShader.Destroy();
        generalVertices.Destroy();
        generalUniforms.Destroy();
        //debug
        simpleShader.Destroy();      
        wirePipeline.Destroy();        
        linesPipeline.Destroy();        
        //terrain
        terrainShader.Destroy();      
        terrainPipeline.Destroy();     
        //foliage
        foliageShader.Destroy();       
        foliagePipeline.Destroy();     
    }

    void Update(gpu::RenderData& renderData, const res::Resources& resources)
    {
        generalUniforms.Update(renderData.general, resources.terrain);
        generalVertices.Update(renderData.general, resources.terrain);
    }

    void RecordTerrain(VkCommandBuffer cmdBuffer, RenderData_General& render_data_general)
    {
        //TODO: bind specific for terrain
        vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, generalPipeline.layout, 0, 
        generalUniforms.descriptors.descSets.count, generalUniforms.descriptors.descSets.data, 0, nullptr);
        vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, terrainPipeline.pipeline);

        vkCmdBindIndexBuffer    (cmdBuffer, generalVertices.iboTerrain.activeBuffer->buffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &generalVertices.vboTerrain.activeBuffer->buffer, generalVertices.offsets);
        vkCmdDrawIndexed        (cmdBuffer, generalVertices.iboTerrain.count, 1, 0, 0 , 0);

        if (render_data_general.enableTerrainWire)
        {
            vkCmdBindPipeline   (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, wirePipeline.pipeline);
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
            vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, generalPipeline.pipeline);

            FOR_C_ARRAY(mesh_array, meshIdx)
            {
                auto const& inst_array  = mesh_array[meshIdx];
                if (inst_array.Empty()) continue;

                auto const& vertexRange = generalVertices.vboMeshesVertexRanges[meshIdx];
                auto const& indexRange  = generalVertices.iboMeshesIndexRanges [meshIdx];
                vkCmdDrawIndexed(cmdBuffer, indexRange.count, inst_array.Count(), indexRange.index, vertexRange.index, 0); //instance idx? buffer offsets?
                dbg::LogInfo(inst_array.Count());
            }
        }
    }

    void Record(VkCommandBuffer cmdBuffer, RenderData_General& render_data_general)
    {
        vkCmdBeginRenderPass(cmdBuffer, &generalRenderPass.beginInfo, VK_SUBPASS_CONTENTS_INLINE);
        RecordTerrain(cmdBuffer, render_data_general);
        RecordInstances(cmdBuffer, render_data_general);
        vkCmdEndRenderPass(cmdBuffer);
    };
    
};

}//ns