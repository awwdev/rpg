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
#include "mini/RenderGraph/UboData.hpp"

namespace mini::vk
{
    using namespace rendergraph;
    using namespace utils;

    struct VkResources
    {
        //host to vk resources
        ImageArray ui_fontImages;

        Default_PushConstants default_pushConsts;

        RenderPass      ui_renderPass;
        Shader          ui_shader;
        Pipeline        ui_pipeline;
        UniformBuffer_Array<UniformData_Text, 1000> ui_ubo_array;

        RenderPass      default_renderPass;
        Shader          default_shader;
        Pipeline        default_pipeline;
        UniformBuffer_Array<UniformData_Default, 1000> default_ubo_array;
        VertexBuffer_Static<Vertex, 1000> default_vbo;


        void Create(hostRes::HostResources& hostRes, Commands& commands)
        {
            //? host to vk resources
            ui_fontImages.Create(hostRes.fontTextures, commands.cmdPool);

            //? "factories"
            CreateUniformBuffer_Text    (ui_ubo_array);
            CreateShader_Text           (ui_shader, ui_fontImages);
            CreateRenderPass_Text       (ui_renderPass);
            CreatePipeline_Text         (ui_pipeline, ui_shader, ui_renderPass, ui_ubo_array);

            CreateVertexBuffer_Default  (default_vbo, commands.cmdPool);
            CreateUniformBuffer_Default (default_ubo_array);
            CreateShader_Default        (default_shader);
            CreateRenderPass_Default    (default_renderPass, commands.cmdPool);
            CreatePipeline_Default      (default_pipeline, default_shader, default_renderPass, default_vbo, default_ubo_array);
        }

        void RecreateSwapchain(VkCommandPool cmdPool)
        {
            ui_pipeline.~Pipeline();
            default_pipeline.~Pipeline();
            default_renderPass.~RenderPass();
            ui_renderPass.~RenderPass();

            CreateRenderPass_Text       (ui_renderPass);
            CreateRenderPass_Default    (default_renderPass, cmdPool);
            CreatePipeline_Default      (default_pipeline, default_shader, default_renderPass, default_vbo, default_ubo_array);
            CreatePipeline_Text         (ui_pipeline, ui_shader, ui_renderPass, ui_ubo_array);
        }
    };

} // namespace mini::vk