//https://github.com/awwdev

#pragma once
#include "Vulkan/Core.hpp"
#include "Vulkan/Context.hpp"
#include "Resources/HostResources.hpp"
#include "Vulkan/Resources/Resources_Default.hpp"
#include "Vulkan/Resources/Resources_Shadow.hpp"
#include "Vulkan/Objects/UniformBuffer.hpp"

#include "Vulkan/Factories/Terrain/Terrain_VertexBuffer.hpp"
#include "Vulkan/Factories/Terrain/Terrain_UniformBuffer.hpp"
#include "Vulkan/Factories/Terrain/Terrain_Pipeline.hpp"
#include "Vulkan/Factories/Terrain/Terrain_PipelineWire.hpp"
#include "Vulkan/Factories/Terrain/Terrain_PipelineShadow.hpp"
#include "Vulkan/Factories/Terrain/Terrain_Shader.hpp" //includes wire and shadow

namespace rpg::vk {

struct Resources_Terrain
{
    Shader      shader          {};
    Shader      shaderShadow    {}; 
    Shader      shaderWire      {};
    Pipeline    pipeline        {};
    Pipeline    pipelineShadow  {};
    Pipeline    pipelineWire    {};

    VertexBuffer<use::Common_Vertex, gpu::TERRAIN_VERTEX_MAX_COUNT> vbo;
    UniformBuffer<gpu::Terrain_UniformData, 1> uboMeta;

    void Create(res::HostResources& hostRes, VkCommandPool cmdPool, Resources_Shadow& shadow, Resources_Common& common)
    {
        Terrain_CreateVertexBuffer  (vbo, cmdPool, hostRes);
        Terrain_CreateUniformBuffer (uboMeta);
        Terrain_CreateShaderShadow  (shaderShadow);
        Terrain_CreateShader        (shader, shadow.renderPass);
        Terrain_CreateShaderWire    (shaderWire);
        Terrain_CreatePipelineShadow(pipelineShadow, shaderShadow, shadow.renderPass, vbo);
        Terrain_CreatePipeline      (pipeline, shader, common.renderPass, vbo, uboMeta);
        Terrain_CreatePipelineWire  (pipelineWire, shaderWire, common.renderPass, vbo);
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
        Terrain_CreatePipeline      (pipeline, shader, common.renderPass, vbo, uboMeta);
        Terrain_CreatePipelineWire  (pipelineWire, shaderWire, common.renderPass, vbo);
    }
};

}//NS