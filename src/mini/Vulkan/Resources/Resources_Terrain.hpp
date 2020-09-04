//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"
#include "mini/Resources/HostResources.hpp"
#include "mini/Vulkan/Resources/Resources_Default.hpp"
#include "mini/Vulkan/Resources/Resources_Shadow.hpp"
#include "mini/Vulkan/Objects/UniformBuffer.hpp"

#include "mini/Vulkan/Factories/Terrain/Terrain_VertexBuffer.hpp"
#include "mini/Vulkan/Factories/Terrain/Terrain_Pipeline.hpp"
#include "mini/Vulkan/Factories/Terrain/Terrain_PipelineWire.hpp"
#include "mini/Vulkan/Factories/Terrain/Terrain_PipelineShadow.hpp"
#include "mini/Vulkan/Factories/Terrain/Terrain_Shader.hpp" //includes wire and shadow

namespace mini::vk {

struct Resources_Terrain
{
    Shader      shader;
    Shader      shaderShadow;
    Shader      shaderWire;
    Pipeline    pipeline;
    Pipeline    pipelineShadow;
    Pipeline    pipelineWire;

    VertexBuffer<utils::Common_Vertex, rendering::TERRAIN_VERTEX_MAX_COUNT> vbo;

    void Create(res::HostResources& hostRes, VkCommandPool cmdPool, Resources_Shadow& shadow, Resources_Default& default)
    {
        Terrain_CreateVertexBuffer  (vbo, cmdPool, hostRes);
        Terrain_CreateShaderShadow  (shaderShadow);
        Terrain_CreateShader        (shader, shadow.renderPass);
        Terrain_CreateShaderWire    (shaderWire);
        Terrain_CreatePipelineShadow(pipelineShadow, shaderShadow, shadow.renderPass, vbo);
        Terrain_CreatePipeline      (pipeline, shader, default.renderPass, vbo);
        Terrain_CreatePipelineWire  (pipelineWire, shaderWire, default.renderPass, vbo);
    }

    void Recreate(VkCommandPool& cmdPool, Resources_Shadow& shadow, Resources_Default& default)
    {
        shader.~Shader();
        pipeline.~Pipeline();
        pipelineWire.~Pipeline();
        pipelineShadow.~Pipeline();
        
        //TODO: recreate whole shader is wrong, only sampler needs recreation (due to img resize)
        Terrain_CreateShader        (shader, shadow.renderPass);
        Terrain_CreatePipelineShadow(pipelineShadow, shaderShadow, shadow.renderPass, vbo);
        Terrain_CreatePipeline      (pipeline, shader, default.renderPass, vbo);
        Terrain_CreatePipelineWire  (pipelineWire, shaderWire, default.renderPass, vbo);
    }
};

}//NS