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
#include "mini/Vulkan/Factories/Default/Default_Shader.hpp"
#include "mini/Vulkan/Factories/Default/Default_VertexBuffer.hpp"
#include "mini/Vulkan/Factories/Default/Default_UniformBuffer.hpp"

#include "mini/Vulkan/Factories/Terrain/Terrain_VertexBuffer.hpp"

#include "mini/Vulkan/Objects/ImageArray.hpp"
#include "mini/Vulkan/Objects/VertexBuffer.hpp"

#include "mini/Resources/HostResources.hpp"
#include "mini/Rendering/UboData.hpp"
#include "mini/Rendering/RenderGraph.hpp"

namespace mini::vk
{
    using namespace rendering;
    using namespace utils;

    struct UI_Resources
    {
        ImageArray fontImages;

        RenderPass renderPass;
        Shader shader;
        Pipeline pipeline;
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

    struct Default_Resources
    {
        RenderPass renderPass;
        Shader shader;
        Pipeline pipeline;
        UniformBuffer_Groups<Default_UniformData, DEFAULT_UBO_MAX_COUNT> ubo;
        VertexBuffer<Common_Vertex, DEFAULT_VERTEX_MAX_COUNT> vbo;

        void Create(res::HostResources& hostRes, Commands& commands)
        {
            Default_CreateVertexBuffer  (vbo, commands.cmdPool, hostRes);
            Default_CreateUniformBuffer (ubo);
            Default_CreateShader        (shader);
            Default_CreateRenderPass    (renderPass, commands.cmdPool);
            Default_CreatePipeline      (pipeline, shader, renderPass, vbo, ubo);
        }
    };

    struct Terrain_Resources
    {
        VertexBuffer<Common_Vertex, TERRAIN_VERTEX_MAX_COUNT> vbo;

        void Create(res::HostResources& hostRes, Commands& commands)
        {
            Terrain_CreateVertexBuffer(vbo, commands.cmdPool, hostRes);
        }
    };

    struct VkResources
    {
        Common_PushConstants common_pushConsts;
        //sub resources
        UI_Resources ui;
        Default_Resources default;
        Terrain_Resources terrain;

        void Create(res::HostResources& hostRes, Commands& commands)
        {
            ui.Create(hostRes, commands);
            default.Create(hostRes, commands);
            terrain.Create(hostRes, commands);
        }

        void RecreateSwapchain(VkCommandPool cmdPool)
        {
            ui.pipeline.~Pipeline();
            ui.renderPass.~RenderPass();
            default.pipeline.~Pipeline();
            default.renderPass.~RenderPass();
            
            UI_CreateRenderPass       (ui.renderPass);
            Default_CreateRenderPass    (default.renderPass, cmdPool);
            Default_CreatePipeline      (default.pipeline, default.shader, default.renderPass, default.vbo, default.ubo);
            UI_CreatePipeline         (ui.pipeline, ui.shader, ui.renderPass, ui.ubo);
        }
    };

} //ns