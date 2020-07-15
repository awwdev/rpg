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
        UniformBuffer_Groups<UniformData_UI, UI_UBO_MAX_COUNT> ubo; //one group only

        void Create(res::HostResources& hostRes, Commands& commands)
        {
            fontImages.Create(hostRes.textures.monospaceFont, commands.cmdPool);

            CreateUniformBuffer_UI    (ubo);
            CreateShader_UI           (shader, fontImages);
            CreateRenderPass_UI       (renderPass);
            CreatePipeline_UI         (pipeline, shader, renderPass, ubo);
        }
    };

    struct Default_Resources
    {
        RenderPass renderPass;
        Shader shader;
        Pipeline pipeline;
        UniformBuffer_Groups<UniformData_Default, DEFAULT_UBO_MAX_COUNT> ubo;
        VertexBuffer_Static<VertexDefault, DEFAULT_VERTEX_MAX_COUNT> vbo; //hardcoded limit 

        void Create(res::HostResources& hostRes, Commands& commands)
        {
            CreateVertexBuffer_Default  (vbo, commands.cmdPool, hostRes);
            CreateUniformBuffer_Default (ubo);
            CreateShader_Default        (shader);
            CreateRenderPass_Default    (renderPass, commands.cmdPool);
            CreatePipeline_Default      (pipeline, shader, renderPass, vbo, ubo);
        }
    };

    struct VkResources
    {
        Common_PushConstants common_pushConsts;
        //sub resources
        UI_Resources ui;
        Default_Resources default;

        void Create(res::HostResources& hostRes, Commands& commands)
        {
            ui.Create(hostRes, commands);
            default.Create(hostRes, commands);
        }

        void RecreateSwapchain(VkCommandPool cmdPool)
        {
            ui.pipeline.~Pipeline();
            default.pipeline.~Pipeline();
            default.renderPass.~RenderPass();
            ui.renderPass.~RenderPass();

            CreateRenderPass_UI       (ui.renderPass);
            CreateRenderPass_Default    (default.renderPass, cmdPool);
            CreatePipeline_Default      (default.pipeline, default.shader, default.renderPass, default.vbo, default.ubo);
            CreatePipeline_UI         (ui.pipeline, ui.shader, ui.renderPass, ui.ubo);
        }
    };

} //ns