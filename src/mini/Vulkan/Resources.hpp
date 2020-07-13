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
        UniformBuffer_Array<UniformData_Text, 1000> ubo_array;

        void Create(hostRes::HostResources& hostRes, Commands& commands)
        {
            fontImages.Create(hostRes.fontTextures, commands.cmdPool);

            CreateUniformBuffer_Text    (ubo_array);
            CreateShader_Text           (shader, fontImages);
            CreateRenderPass_Text       (renderPass);
            CreatePipeline_Text         (pipeline, shader, renderPass, ubo_array);
        }
    };

    struct Default_Resources
    {
        RenderPass renderPass;
        Shader shader;
        Pipeline pipeline;
        UniformBuffer_Groups<UniformData_Default, 500> ubo_groups;
        VertexBuffer_Static<Vertex, 1000> vbo;

        void Create(hostRes::HostResources& hostRes, Commands& commands)
        {
            CreateVertexBuffer_Default  (vbo, commands.cmdPool);
            CreateUniformBuffer_Default (ubo_groups);
            CreateShader_Default        (shader);
            CreateRenderPass_Default    (renderPass, commands.cmdPool);
            CreatePipeline_Default      (pipeline, shader, renderPass, vbo, ubo_groups);
        }
    };

    struct VkResources
    {
        Common_PushConstants common_pushConsts;
        //sub resources
        UI_Resources ui;
        Default_Resources default;

        void Create(hostRes::HostResources& hostRes, Commands& commands)
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

            CreateRenderPass_Text       (ui.renderPass);
            CreateRenderPass_Default    (default.renderPass, cmdPool);
            CreatePipeline_Default      (default.pipeline, default.shader, default.renderPass, default.vbo, default.ubo_groups);
            CreatePipeline_Text         (ui.pipeline, ui.shader, ui.renderPass, ui.ubo_array);
        }
    };

} //ns