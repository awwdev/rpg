//https://github.com/awwdev

#pragma once

#include "gpu/Vulkan/States/Resources/Resources_Shaders.hpp"
#include "gpu/Vulkan/States/Resources/Resources_RenderPasses.hpp"
#include "gpu/Vulkan/States/Resources/Resources_Uniforms.hpp"
#include "gpu/Vulkan/States/Resources/Resources_Vertices.hpp"

#include "gpu/Vulkan/States/Resources/Pipelines/Pipeline_Shadow.hpp"
#include "gpu/Vulkan/States/Resources/Pipelines/Pipeline_Terrain.hpp"
#include "gpu/Vulkan/States/Resources/Pipelines/Pipeline_TerrainWire.hpp"
#include "gpu/Vulkan/States/Resources/Pipelines/Pipeline_General.hpp"
#include "gpu/Vulkan/States/Resources/Pipelines/Pipeline_Line.hpp"
#include "gpu/Vulkan/States/Resources/Pipelines/Pipeline_Foliage.hpp"
#include "gpu/Vulkan/States/Resources/Pipelines/Pipeline_GUI.hpp"
#include "gpu/Vulkan/States/Resources/Pipelines/Pipeline_Post.hpp"

namespace rpg::gpu::vuk {

struct Resources_Pipelines
{
    //shadow
    Pipeline shadowPipeline;
    //terrain
    Pipeline terrainPipeline;
    Pipeline terrainWirePipeline;
    //instances
    Pipeline generalPipeline;
    Pipeline foliagePipeline;
    Pipeline linePipeline;
    //post
    Pipeline guiPipeline;
    Pipeline postPipeline;

    void Create(
        Resources_Shaders& shaders, 
        Resources_RenderPasses& renderPasses, 
        Resources_Uniforms& uniforms, 
        Resources_Vertices& vertices)
    {
        //shadow
        CreatePipeline_Shadow       (shadowPipeline, shaders.shadowShader, renderPasses.shadowRenderPass);
        //terrain
        CreatePipeline_Terrain      (terrainPipeline, shaders.terrainShader, renderPasses.mainRenderPass);
        CreatePipeline_TerrainWire  (terrainWirePipeline, shaders.terrainWireShader, renderPasses.mainRenderPass);
        //instance
        CreatePipeline_General      (generalPipeline, shaders.generalShader, renderPasses.mainRenderPass);
        CreatePipeline_Foliage      (foliagePipeline, shaders.foliageShader, renderPasses.mainRenderPass);
        CreatePipeline_Line         (linePipeline, shaders.lineShader, renderPasses.mainRenderPass);
        //post
        CreatePipeline_GUI          (guiPipeline, shaders.guiShader, renderPasses.postRenderPass);
        CreatePipeline_Post         (postPipeline, shaders.postShader, renderPasses.postRenderPass);
    }

    void Destroy()
    {
        //post
        guiPipeline.Destroy();
        postPipeline.Destroy();
        //instance
        generalPipeline.Destroy();
        foliagePipeline.Destroy();
        linePipeline.Destroy();
        //terrain
        terrainPipeline.Destroy();
        terrainWirePipeline.Destroy();
        //shadow
        shadowPipeline.Destroy();
    }
};

}//ns