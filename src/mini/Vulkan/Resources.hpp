//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"

#include "mini/Vulkan/Commands.hpp"
#include "mini/Vulkan/Synchronization.hpp"

#include "mini/Vulkan/Factories/UI/UI_RenderPass.hpp"
#include "mini/Vulkan/Factories/UI/UI_Pipeline.hpp"
#include "mini/Vulkan/Factories/UI/UI_Shader.hpp"
#include "mini/Vulkan/Factories/UI/UI_UniformBuffer.hpp"

#include "mini/Vulkan/Factories/Default/Default_RenderPass.hpp"
#include "mini/Vulkan/Factories/Default/Default_PipelineVertexColor.hpp"
#include "mini/Vulkan/Factories/Default/Default_PipelineTexture.hpp"
#include "mini/Vulkan/Factories/Default/Default_PipelineShadow.hpp"
#include "mini/Vulkan/Factories/Default/Default_Shader.hpp" //includes shadow
#include "mini/Vulkan/Factories/Default/Default_VertexBuffer.hpp"
#include "mini/Vulkan/Factories/Default/Default_UniformBuffer.hpp"

#include "mini/Vulkan/Factories/Terrain/Terrain_VertexBuffer.hpp"
#include "mini/Vulkan/Factories/Terrain/Terrain_Pipeline.hpp"
#include "mini/Vulkan/Factories/Terrain/Terrain_PipelineWire.hpp"
#include "mini/Vulkan/Factories/Terrain/Terrain_PipelineShadow.hpp"
#include "mini/Vulkan/Factories/Terrain/Terrain_Shader.hpp" //includes wire and shadow

#include "mini/Vulkan/Factories/Shadow/Shadow_Shader.hpp"
#include "mini/Vulkan/Factories/Shadow/Shadow_RenderPass.hpp"

#include "mini/Vulkan/Factories/Sky/Sky_Pipeline.hpp"
#include "mini/Vulkan/Factories/Sky/Sky_Shader.hpp" 

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

        void Create(res::HostResources& hostRes, VkCommandPool cmdPool)
        {
            fontImages.Create(hostRes.textures.monospaceFont, cmdPool);

            UI_CreateUniformBuffer    (ubo);
            UI_CreateShader           (shader, fontImages);
            UI_CreateRenderPass       (renderPass);
            UI_CreatePipeline         (pipeline, shader, renderPass, ubo);
        }

        void Recreate()
        {
            renderPass.~RenderPass();
            pipeline.~Pipeline();
            UI_CreateRenderPass       (renderPass);
            UI_CreatePipeline         (pipeline, shader, renderPass, ubo);
        }
    };

    struct Resources_Shadow
    {
        RenderPassDepth renderPass;

        void Create(res::HostResources& hostRes, VkCommandPool cmdPool)
        {
            Shadow_CreateRenderPass(renderPass, cmdPool);
        }

        void Recreate(VkCommandPool cmdPool)
        {
            renderPass.~RenderPassDepth();
            Shadow_CreateRenderPass(renderPass, cmdPool);
        }
    };

    struct Resources_Default
    {
        RenderPass  renderPass;
        ImageArray  textures;

        Shader      shaderVertexColor;
        Shader      shaderTexture;
        Shader      shaderShadow;

        Pipeline    pipelineVertexColor;
        Pipeline    pipelineTexture;
        Pipeline    pipelineShadow;

        UniformBuffer_Groups<Default_UniformData, DEFAULT_UBO_MAX_COUNT> ubo;
        VertexBuffer<Common_Vertex, DEFAULT_VERTEX_MAX_COUNT> vbo;

        void Create(res::HostResources& hostRes, VkCommandPool cmdPool, Resources_Shadow& shadow)
        {
            textures.Create(hostRes.textures.default, cmdPool);

            Default_CreateVertexBuffer      (   vbo, cmdPool, hostRes);
            Default_CreateUniformBuffer         (ubo);
            Default_CreateShaderVertexColor     (shaderVertexColor, shadow.renderPass);
            Default_CreateShaderTexture         (shaderTexture, shadow.renderPass, textures);
            Default_CreateShaderShadow          (shaderShadow);
            Default_CreateRenderPass            (renderPass, cmdPool);
            Default_CreatePipelineVertexColor   (pipelineVertexColor, shaderVertexColor, renderPass, vbo, ubo);
            Default_CreatePipelineTexture       (pipelineTexture, shaderTexture, renderPass, vbo, ubo);
            Default_CreatePipelineShadow        (pipelineShadow, shaderShadow, shadow.renderPass, vbo, ubo);
        }

        void Recreate(VkCommandPool cmdPool, Resources_Shadow& shadow)
        {
            shaderVertexColor.~Shader();
            shaderTexture.~Shader();
            pipelineVertexColor.~Pipeline();
            pipelineTexture.~Pipeline();
            pipelineShadow.~Pipeline();
            renderPass.~RenderPass();

            //TODO: recreate whole shader is wrong, only sampler needs recreation (due to img resize)
            Default_CreateShaderVertexColor (shaderVertexColor, shadow.renderPass);
            Default_CreateShaderTexture     (shaderTexture, shadow.renderPass, textures);
            Default_CreateRenderPass        (renderPass, cmdPool);
            Default_CreatePipelineVertexColor          (pipelineVertexColor, shaderVertexColor, renderPass, vbo, ubo);
            Default_CreatePipelineTexture   (pipelineTexture, shaderTexture, renderPass, vbo, ubo);
            Default_CreatePipelineShadow    (pipelineShadow, shaderShadow, shadow.renderPass, vbo, ubo);
        }
    };

    struct Resources_Terrain
    {
        Shader      shader;
        Shader      shaderShadow;
        Shader      shaderWire;
        Pipeline    pipeline;
        Pipeline    pipelineShadow;
        Pipeline    pipelineWire;
        VertexBuffer<Common_Vertex, TERRAIN_VERTEX_MAX_COUNT> vbo;

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

    struct Resources_Sky
    {
        Shader            shader;
        Pipeline          pipeline;
        Sky_PushConstants pushConsts;


        void Create(VkCommandPool& cmdPool, Resources_Default& default)
        {
            Sky_CreateShader        (shader);
            Sky_CreatePipeline      (pipeline, shader, default.renderPass);
        }

        void Recreate(VkCommandPool& cmdPool, Resources_Default& default)
        {
            shader.~Shader();
            pipeline.~Pipeline();

            Sky_CreateShader        (shader);
            Sky_CreatePipeline      (pipeline, shader, default.renderPass);
        }
    };

    struct VkResources
    {
        Common_PushConstants common_pushConsts;

        Resources_Shadow  shadow;
        Resources_Sky     sky;
        Resources_Terrain terrain;
        Resources_Default default;
        Resources_UI      ui;

        void Create(res::HostResources& hostRes, VkCommandPool cmdPool)
        {
            shadow.Create(hostRes, cmdPool);
            default.Create(hostRes, cmdPool, shadow);
            terrain.Create(hostRes, cmdPool, shadow, default);
            sky.Create(cmdPool, default);
            ui.Create(hostRes, cmdPool);
        }

        void RecreateSwapchain(VkCommandPool cmdPool)
        {
            shadow.Recreate(cmdPool);
            default.Recreate(cmdPool, shadow);
            terrain.Recreate(cmdPool, shadow, default);    
            sky.Recreate(cmdPool, default);        
            ui.Recreate();
        }
        
    };

} //ns