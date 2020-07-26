//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"

#include "mini/Vulkan/Commands.hpp"
#include "mini/Vulkan/Synchronization.hpp"

#include "mini/Vulkan/Factories/UI/UI_RenderPass.hpp"
#include "mini/Vulkan/Factories/UI/UI_Pipeline.hpp"
#include "mini/Vulkan/Factories/UI/UI_Shader.hpp"
#include "mini/Vulkan/Factories/UI/UI_VertexBuffer.hpp"
#include "mini/Vulkan/Factories/UI/UI_UniformBuffer.hpp"

#include "mini/Vulkan/Factories/Default/Default_RenderPass.hpp"
#include "mini/Vulkan/Factories/Default/Default_Pipeline.hpp"
#include "mini/Vulkan/Factories/Default/Default_PipelineShadow.hpp"
#include "mini/Vulkan/Factories/Default/Default_Shader.hpp" //includes shadow
#include "mini/Vulkan/Factories/Default/Default_VertexBuffer.hpp"
#include "mini/Vulkan/Factories/Default/Default_UniformBuffer.hpp"

#include "mini/Vulkan/Factories/Terrain/Terrain_VertexBuffer.hpp"
#include "mini/Vulkan/Factories/Terrain/Terrain_Pipeline.hpp"
#include "mini/Vulkan/Factories/Terrain/Terrain_PipelineWire.hpp"
#include "mini/Vulkan/Factories/Terrain/Terrain_PipelineShadow.hpp"
#include "mini/Vulkan/Factories/Terrain/Terrain_RenderPass.hpp"
#include "mini/Vulkan/Factories/Terrain/Terrain_Shader.hpp" //includes wire and shadow

#include "mini/Vulkan/Factories/Shadow/Shadow_Pipeline.hpp"
#include "mini/Vulkan/Factories/Shadow/Shadow_Shader.hpp"
#include "mini/Vulkan/Factories/Shadow/Shadow_RenderPass.hpp"

#include "mini/Vulkan/Objects/ImageArray.hpp"
#include "mini/Vulkan/Objects/VertexBuffer.hpp"

#include "mini/Resources/HostResources.hpp"
#include "mini/Rendering/UboData.hpp"
#include "mini/Rendering/RenderGraph.hpp"

namespace mini::vk
{
    using namespace rendering;
    using namespace utils;

    struct Resources_UI
    {
        ImageArray fontImages;

        UI_PushConstants pushConsts;
        RenderPass       renderPass;
        Shader           shader;
        Pipeline         pipeline;
        UniformBuffer_Groups<UI_UniformData, UI_UBO_MAX_COUNT> ubo; //one group only

        void Create(res::HostResources& hostRes, Commands& commands)
        {
            fontImages.Create(hostRes.textures.monospaceFont, commands.cmdPool);

            UI_CreateUniformBuffer    (ubo);
            UI_CreateShader           (shader, fontImages);
            UI_CreateRenderPass       (renderPass);
            UI_CreatePipeline         (pipeline, shader, renderPass, ubo);
        }
    };

    struct Resources_Shadow
    {
        RenderPassDepth renderPass;

        void Create(res::HostResources& hostRes, Commands& commands)
        {
            Shadow_CreateRenderPass(renderPass, commands.cmdPool);
        }
    };

    struct Resources_Default
    {
        RenderPass  renderPass;
        Shader      shader;
        Shader      shaderShadow;
        Pipeline    pipeline;
        Pipeline    pipelineShadow;
        UniformBuffer_Groups<Default_UniformData, DEFAULT_UBO_MAX_COUNT> ubo;
        VertexBuffer<Common_Vertex, DEFAULT_VERTEX_MAX_COUNT> vbo;

        void Create(res::HostResources& hostRes, Commands& commands, Resources_Shadow& shadow)
        {
            Default_CreateVertexBuffer  (vbo, commands.cmdPool, hostRes);
            Default_CreateUniformBuffer (ubo);
            Default_CreateShader        (shader);
            Default_CreateShaderShadow  (shaderShadow);
            Default_CreateRenderPass    (renderPass, commands.cmdPool);
            Default_CreatePipeline      (pipeline, shader, renderPass, vbo, ubo);
            Default_CreatePipelineShadow(pipelineShadow, shaderShadow, shadow.renderPass, vbo, ubo);
        }
    };

    struct Resources_Terrain
    {
        RenderPass  renderPass;
        Shader      shader;
        Shader      shaderShadow;
        Shader      shaderWire;
        Pipeline    pipeline;
        Pipeline    pipelineShadow;
        Pipeline    pipelineWire;
        VertexBuffer<Common_Vertex, TERRAIN_VERTEX_MAX_COUNT> vbo;

        void Create(res::HostResources& hostRes, Commands& commands, Resources_Shadow& shadow)
        {
            Terrain_CreateVertexBuffer  (vbo, commands.cmdPool, hostRes);
            Terrain_CreateShaderShadow  (shaderShadow);
            Terrain_CreateShader        (shader, shadow.renderPass);
            Terrain_CreateShaderWire    (shaderWire);
            Terrain_CreateRenderPass    (renderPass, commands.cmdPool);
            Terrain_CreatePipelineShadow(pipelineShadow, shaderShadow, shadow.renderPass, vbo);
            Terrain_CreatePipeline      (pipeline, shader, renderPass, vbo);
            Terrain_CreatePipelineWire  (pipelineWire, shaderWire, renderPass, vbo);
        }
    };

    struct VkResources
    {
        Common_PushConstants common_pushConsts;

        Resources_UI      ui;
        Resources_Default default;
        Resources_Terrain terrain;
        Resources_Shadow  shadow;

        void Create(res::HostResources& hostRes, Commands& commands)
        {
            ui.Create(hostRes, commands);
            shadow.Create(hostRes, commands);
            default.Create(hostRes, commands, shadow);
            terrain.Create(hostRes, commands, shadow);
        }

        void RecreateSwapchain(VkCommandPool cmdPool)
        {
            shadow.renderPass.~RenderPassDepth();

            ui.pipeline.~Pipeline();
            ui.renderPass.~RenderPass();

            default.pipeline.~Pipeline();
            default.pipelineShadow.~Pipeline();
            default.renderPass.~RenderPass();

            terrain.pipeline.~Pipeline();
            terrain.pipelineWire.~Pipeline();
            terrain.pipelineShadow.~Pipeline();
            terrain.renderPass.~RenderPass();
            
            Shadow_CreateRenderPass     (shadow.renderPass, cmdPool);

            Default_CreateRenderPass    (default.renderPass, cmdPool);
            Default_CreatePipeline      (default.pipeline, default.shader, default.renderPass, default.vbo, default.ubo);
            Default_CreatePipelineShadow(default.pipeline, default.shaderShadow, shadow.renderPass, default.vbo, default.ubo);

            UI_CreateRenderPass         (ui.renderPass);
            UI_CreatePipeline           (ui.pipeline, ui.shader, ui.renderPass, ui.ubo);

            Terrain_CreateRenderPass    (terrain.renderPass, cmdPool);
            Terrain_CreatePipeline      (terrain.pipeline, terrain.shader, terrain.renderPass, terrain.vbo);
            Terrain_CreatePipelineWire  (terrain.pipelineWire, terrain.shaderWire, terrain.renderPass, terrain.vbo);
            Terrain_CreatePipelineShadow(terrain.pipelineShadow, terrain.shaderShadow, shadow.renderPass, terrain.vbo);
        }
    };

} //ns