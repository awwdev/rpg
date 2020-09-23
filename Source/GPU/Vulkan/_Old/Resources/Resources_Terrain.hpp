//https://github.com/awwdev

#pragma once

#include "gpu/Vulkan/Meta/Context.hpp"
#include "res/_Old/CpuResources.hpp"
#include "gpu/Vulkan/_Old/res/Resources_Default.hpp"
#include "gpu/Vulkan/_Old/res/Resources_Shadow.hpp"
#include "gpu/Vulkan/_Old/Objects/UniformBuffer.hpp"

#include "gpu/Vulkan/_Old/Factories/Terrain/Terrain_VertexBuffer.hpp"
#include "gpu/Vulkan/_Old/Factories/Terrain/Terrain_UniformBuffer.hpp"
#include "gpu/Vulkan/_Old/Factories/Terrain/Terrain_Pipeline.hpp"
#include "gpu/Vulkan/_Old/Factories/Terrain/Terrain_PipelineWire.hpp"
#include "gpu/Vulkan/_Old/Factories/Terrain/Terrain_PipelineShadow.hpp"
#include "gpu/Vulkan/_Old/Factories/Terrain/Terrain_Shader.hpp" //includes wire and shadow

namespace rpg::gpu::vuk {

struct Resources_Terrain
{
    Shader      shader          {};
    Shader      shaderShadow    {}; 
    Shader      shaderWire      {};
    Pipeline    pipeline        {};
    Pipeline    pipelineShadow  {};
    Pipeline    pipelineWire    {};

    VertexBufferOld<com::Common_Vertex, gpu::TERRAIN_VERTEX_MAX_COUNT> vbo;
    UniformBufferOld<gpu::Terrain_UniformData, 1> uboMeta;

    void Create(res::CpuResources& hostRes, VkCommandPool cmdPool, Resources_Shadow& shadow, Resources_Common& common)
    {
        Terrain_CreateVertexBuffer  (vbo, cmdPool, hostRes);
        Terrain_CreateUniformBuffer (uboMeta);
        Terrain_CreateShaderShadow  (shaderShadow);
        Terrain_CreateShader        (shader, shadow.renderPass);
        Terrain_CreateShaderWire    (shaderWire);
        Terrain_CreatePipelineShadow(pipelineShadow, shaderShadow, shadow.renderPass, vbo);
        //Terrain_CreatePipeline      (pipeline, shader, common.renderPass, vbo, uboMeta);
        //Terrain_CreatePipelineWire  (pipelineWire, shaderWire, common.renderPass, vbo);
    }

    void Recreate(VkCommandPool&, Resources_Shadow& shadow, Resources_Common& common)
    {
        shader.Clear();
        pipeline.Clear();
        pipelineWire.Clear();
        pipelineShadow.Clear();

        //TODO: recreate whole shader is wrong, only sampler needs recreation (due to img resize)
        Terrain_CreateShader        (shader, shadow.renderPass);
        Terrain_CreatePipelineShadow(pipelineShadow, shaderShadow, shadow.renderPass, vbo);
        //Terrain_CreatePipeline      (pipeline, shader, common.renderPass, vbo, uboMeta);
        //Terrain_CreatePipelineWire  (pipelineWire, shaderWire, common.renderPass, vbo);
    }
};

}//NS