//https://github.com/awwdev

#pragma once
#include "gpu/RenderData/RenderData.hpp"
#include "res/Resources.hpp"

//general
#include "gpu/Vulkan/_Old/Main/Main_RenderPass.hpp"
#include "gpu/Vulkan/_Old/Main/Main_Vertices.hpp"
#include "gpu/Vulkan/_Old/Main/Main_Uniforms.hpp"
//terrain
#include "gpu/Vulkan/_Old/Main/Terrain/Terrain_Shader.hpp"
#include "gpu/Vulkan/_Old/Main/Terrain/Terrain_Pipeline.hpp"
#include "gpu/Vulkan/_Old/Main/Terrain/TerrainWire_Shader.hpp"
#include "gpu/Vulkan/_Old/Main/Terrain/TerrainWire_Pipeline.hpp"
//instances
#include "gpu/Vulkan/_Old/Main/Instances/General/General_Pipeline.hpp"
#include "gpu/Vulkan/_Old/Main/Instances/General/General_Shader.hpp"
#include "gpu/Vulkan/_Old/Main/Instances/Foliage/Foliage_Pipeline.hpp"
#include "gpu/Vulkan/_Old/Main/Instances/Foliage/Foliage_Shader.hpp"
#include "gpu/Vulkan/_Old/Main/Instances/Line/Line_Pipeline.hpp"
#include "gpu/Vulkan/_Old/Main/Instances/Line/Line_Shader.hpp"

namespace rpg::gpu::vuk {

struct Pass_Main
{
    //general
    Main_RenderPass      mainRenderPass;
    Main_Vertices        mainVertices;
    Main_Uniforms        mainUniforms;
    //terrain
    Terrain_Shader       terrainShader;
    Terrain_Pipeline     terrainPipeline;
    TerrainWire_Shader   terrainWireShader;
    TerrainWire_Pipeline terrainWirePipeline;
    //instances (materials)
    General_Shader       generalShader;
    General_Pipeline     generalPipeline;
    Foliage_Shader       foliageShader;
    Foliage_Pipeline     foliagePipeline;
    Line_Shader          lineShader;
    Line_Pipeline        linePipeline;

    void Create(VkCommandPool cmdPool, Buffer& uboSun, Image& shadowMaps, res::Resources& res)
    {
        //main
        mainVertices.Create(cmdPool, res.meshes, res.terrain);
        mainUniforms.Create(cmdPool, uboSun, shadowMaps, res.textures);
        mainRenderPass.Create(cmdPool);
        //terrain
        terrainShader.Create();
        terrainPipeline.Create(mainRenderPass, terrainShader, mainVertices, mainUniforms);
        terrainWireShader.Create();
        terrainWirePipeline .Create(mainRenderPass, terrainWireShader, mainVertices, mainUniforms);
        //instances (materials)
        generalShader.Create();
        generalPipeline.Create(mainRenderPass, generalShader, mainVertices, mainUniforms);
        foliageShader.Create();
        foliagePipeline.Create(mainRenderPass, foliageShader, mainVertices, mainUniforms);
        lineShader.Create();
        linePipeline.Create(mainRenderPass, lineShader, mainVertices, mainUniforms);
    }

    void Destroy()
    {
        //main
        mainRenderPass.Destroy();
        mainVertices.Destroy();
        mainUniforms.Destroy();
        //terrain
        terrainShader.Destroy();      
        terrainPipeline.Destroy();     
        terrainWireShader.Destroy();      
        terrainWirePipeline.Destroy();  
        //instances  
        generalShader.Destroy();      
        generalPipeline.Destroy();  
        foliageShader.Destroy();      
        foliagePipeline.Destroy();  
        lineShader.Destroy();      
        linePipeline.Destroy();  
    }

    void Update(gpu::RenderData& renderData, const res::Resources& resources)
    {
        mainUniforms.Update(renderData.main, resources.terrain);
        mainVertices.Update(renderData.main, resources.terrain);
    }

    void Record(VkCommandBuffer cmdBuffer, RenderData_Main& renderDataMain)
    {
        vkCmdBeginRenderPass(cmdBuffer, &mainRenderPass.beginInfo, VK_SUBPASS_CONTENTS_INLINE);
        RecordTerrain(cmdBuffer, renderDataMain);
        RecordInstances(cmdBuffer, renderDataMain);
        vkCmdEndRenderPass(cmdBuffer);
    };

    void RecordTerrain(VkCommandBuffer cmdBuffer, RenderData_Main& renderDataMain)
    {
        vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, terrainPipeline.pipeline);
        vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, terrainPipeline.layout, 0, 
        mainUniforms.descriptors.descSets.count, mainUniforms.descriptors.descSets.data, 0, nullptr);

        vkCmdBindIndexBuffer    (cmdBuffer, mainVertices.iboTerrain.activeBuffer->buffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &mainVertices.vboTerrain.activeBuffer->buffer, mainVertices.offsets);
        vkCmdDrawIndexed        (cmdBuffer, mainVertices.iboTerrain.count, 1, 0, 0 , 0);

        if (renderDataMain.enableTerrainWire)
        {
            vkCmdBindPipeline   (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, terrainWirePipeline.pipeline);
            vkCmdDrawIndexed    (cmdBuffer, mainVertices.iboTerrain.count, 1, 0, 0 , 0);
        }
    }

    void RecordInstances(VkCommandBuffer cmdBuffer, RenderData_Main& renderDataMain)
    {
        vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &mainVertices.vboMeshes.activeBuffer->buffer, mainVertices.offsets);
        vkCmdBindIndexBuffer    (cmdBuffer, mainVertices.iboMeshes.activeBuffer->buffer, 0, VkIndexType::VK_INDEX_TYPE_UINT32);

        //materials->meshes->instances
        auto const& materials = renderDataMain.instance_datas;
        FOR_C_ARRAY(materials, matIdx)
        {
            auto const& meshes = materials[matIdx];

            //bind pipeline 
            switch((res::MeshMaterialEnum)matIdx)
            {
                case res::MeshMaterialEnum::Foliage:    
                vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, foliagePipeline.pipeline);
                break;

                case res::MeshMaterialEnum::Metallic:   
                vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, generalPipeline.pipeline);
                break;

                case res::MeshMaterialEnum::General:  
                vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, generalPipeline.pipeline);
                break;

                case res::MeshMaterialEnum::Line:      
                vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, linePipeline.pipeline);
                break;

                default: continue;
            }

            uint32_t instanceIdx = 0;
            FOR_C_ARRAY(meshes, meshIdx)
            {
                auto const& instances  = meshes[meshIdx];
                if (instances.Empty()) continue;

                auto const& vertexRange = mainVertices.vboMeshesVertexRanges[meshIdx];
                auto const& indexRange  = mainVertices.iboMeshesIndexRanges [meshIdx];
                auto const  instanceCount = instances.Count();
                vkCmdDrawIndexed(cmdBuffer, indexRange.count, instanceCount, indexRange.index, vertexRange.index, instanceIdx);
                instanceIdx += instanceCount;
            }
        }
    }
    
};

}//ns