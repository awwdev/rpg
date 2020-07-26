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
        Shader      shader;
        Shader      shaderShadow;
        Pipeline    pipeline;
        Pipeline    pipelineShadow;
        UniformBuffer_Groups<Default_UniformData, DEFAULT_UBO_MAX_COUNT> ubo;
        VertexBuffer<Common_Vertex, DEFAULT_VERTEX_MAX_COUNT> vbo;

        void Create(res::HostResources& hostRes, VkCommandPool cmdPool, Resources_Shadow& shadow)
        {
            Default_CreateVertexBuffer  (vbo, cmdPool, hostRes);
            Default_CreateUniformBuffer (ubo);
            Default_CreateShader        (shader);
            Default_CreateShaderShadow  (shaderShadow);
            Default_CreateRenderPass    (renderPass, cmdPool);
            Default_CreatePipeline      (pipeline, shader, renderPass, vbo, ubo);
            Default_CreatePipelineShadow(pipelineShadow, shaderShadow, shadow.renderPass, vbo, ubo);
        }

        void Recreate(VkCommandPool cmdPool, Resources_Shadow& shadow)
        {
            pipeline.~Pipeline();
            pipelineShadow.~Pipeline();
            renderPass.~RenderPass();

            Default_CreateRenderPass    (renderPass, cmdPool);
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

        void Create(res::HostResources& hostRes, VkCommandPool cmdPool, Resources_Shadow& shadow)
        {
            Terrain_CreateVertexBuffer  (vbo, cmdPool, hostRes);
            Terrain_CreateShaderShadow  (shaderShadow);
            Terrain_CreateShader        (shader, shadow.renderPass);
            Terrain_CreateShaderWire    (shaderWire);
            Terrain_CreateRenderPass    (renderPass, cmdPool);
            Terrain_CreatePipelineShadow(pipelineShadow, shaderShadow, shadow.renderPass, vbo);
            Terrain_CreatePipeline      (pipeline, shader, renderPass, vbo);
            Terrain_CreatePipelineWire  (pipelineWire, shaderWire, renderPass, vbo);
        }

        void Recreate(VkCommandPool& cmdPool, Resources_Shadow& shadow)
        {
            pipeline.~Pipeline();
            pipelineWire.~Pipeline();
            pipelineShadow.~Pipeline();
            renderPass.~RenderPass();

            Terrain_CreateRenderPass    (renderPass, cmdPool);
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

        void Create(res::HostResources& hostRes, VkCommandPool cmdPool)
        {
            ui.Create(hostRes, cmdPool);
            shadow.Create(hostRes, cmdPool);
            default.Create(hostRes, cmdPool, shadow);
            terrain.Create(hostRes, cmdPool, shadow);
        }

        void RecreateSwapchain(VkCommandPool cmdPool)
        {
            ui.Recreate();
            shadow.Recreate(cmdPool);
            default.Recreate(cmdPool, shadow);
            terrain.Recreate(cmdPool, shadow);            
        }
    };

} //ns