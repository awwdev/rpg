//https://github.com/awwdev

#pragma once

#include "gpu/Vulkan/Resources/Pipelines/Pipeline_Shadow.hpp"
#include "gpu/Vulkan/Resources/Pipelines/Pipeline_Terrain.hpp"
#include "gpu/Vulkan/Resources/Pipelines/Pipeline_TerrainWire.hpp"
#include "gpu/Vulkan/Resources/Pipelines/Pipeline_General.hpp"
#include "gpu/Vulkan/Resources/Pipelines/Pipeline_Line.hpp"
#include "gpu/Vulkan/Resources/Pipelines/Pipeline_Foliage.hpp"
#include "gpu/Vulkan/Resources/Pipelines/Pipeline_GUI.hpp"
#include "gpu/Vulkan/Resources/Pipelines/Pipeline_Post.hpp"

namespace rpg::gpu::vuk {

struct Resources_Pipelines
{
    Pipeline guiPipeline;
    Pipeline postPipeline;
    Pipeline shadowPipeline;
    //terrain
    Pipeline terrainPipeline;
    Pipeline terrainWirePipeline;
    //instances
    Pipeline generalPipeline;
    Pipeline foliagePipeline;
    Pipeline linePipeline;

    void Create(Resources_Shaders& shaders)
    {
        CreatePipeline_GUI(shaders.guiShader);
        CreatePipeline_Post(shaders.postShader);
        CreatePipeline_Shadow(shaders.shadowShader);
        //terrain
        CreatePipeline_Terrain(shaders.terrainShader);
        CreatePipeline_TerrainWire(shaders.terrainWireShader);
        //instance
        CreatePipeline_General(shaders.generalShader);
        CreatePipeline_Foliage(shaders.foliageShader);
        CreatePipeline_Line(shaders.lineShader);
    }

    void Destroy()
    {
        guiPipeline.Destroy();
        postPipeline.Destroy();
        shadowPipeline.Destroy();
        //terrain
        terrainPipeline.Destroy();
        terrainWirePipeline.Destroy();
        //instance
        generalPipeline.Destroy();
        foliagePipeline.Destroy();
        linePipeline.Destroy();
    }
};

}//ns