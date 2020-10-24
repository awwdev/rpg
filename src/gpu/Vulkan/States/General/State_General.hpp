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
        linesPipeline       .Create(generalRenderPass, simpleShader, generalVertices, generalUniforms);
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
        generalUniforms.Update(renderData.general);
        generalVertices.Update(renderData.general, resources.terrain);
    }

    void Record(VkCommandBuffer cmdBuffer, RenderData_General& rdGeneral)
    {
        vkCmdBeginRenderPass    (cmdBuffer, &generalRenderPass.beginInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, generalPipeline.layout, 0, 
                                 generalUniforms.descriptors.descSets.count, generalUniforms.descriptors.descSets.data, 0, nullptr);

        //?terrain
        vkCmdBindIndexBuffer    (cmdBuffer, generalVertices.iboTerrain.activeBuffer->buffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &generalVertices.vboTerrain.activeBuffer->buffer, generalVertices.offsets);
        vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, terrainPipeline.pipeline);
        vkCmdDrawIndexed        (cmdBuffer, generalVertices.iboTerrain.count, 1, 0, 0 , 0);

        rdGeneral.dbgVertCountTerrain += generalVertices.vboTerrain.count;

        //?terrain wire
        if (rdGeneral.enableTerrainWire)
        {
            vkCmdBindPipeline   (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, wirePipeline.pipeline);
            vkCmdDrawIndexed    (cmdBuffer, generalVertices.iboTerrain.count, 1, 0, 0 , 0);
        }

        rdGeneral.dbgVertCountTerrain += generalVertices.vboTerrain.count;

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
                    vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, foliagePipeline.pipeline);
                break;

                case res::MeshMaterialEnum::Line: 
                     vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, linesPipeline.pipeline);
                break;

                case res::MeshMaterialEnum::Metal: 
                case res::MeshMaterialEnum::Default: 
                    vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, generalPipeline.pipeline);
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