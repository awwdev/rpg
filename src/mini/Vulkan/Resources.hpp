//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"

#include "mini/Vulkan/Commands.hpp"
#include "mini/Vulkan/Synchronization.hpp"

#include "mini/Vulkan/Factories/UI_RenderPass.hpp"
#include "mini/Vulkan/Factories/UI_Pipeline.hpp"
#include "mini/Vulkan/Factories/UI_Shader.hpp"
#include "mini/Vulkan/Factories/UI_VertexBuffer.hpp"
#include "mini/Vulkan/Factories/UI_UniformBuffer.hpp"
#include "mini/Vulkan/Objects/ImageArray.hpp"

#include "mini/Memory/Allocator.hpp"
#include "mini/Resources/HostResources.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/RenderGraph/UboData.hpp"

namespace mini::vk
{
    struct VkResources
    {
        //? host to vk resources
        ImageArray ui_fontImages;

        //? vk pipeline resources
        Default_PushConstants   default_pushConsts;
        //simple UI (text and quads)
        RenderPass              ui_renderPass;
        Shader                  ui_shader;
        Pipeline                ui_pipeline;
        UniformBuffer_Array<rendergraph::UniformData_Text, 1000> ui_ubo_array;
        //3d default
        //RenderPass              default_renderPass;
        //Shader                  default_shader;
        //Pipeline                default_pipeline;

        //! resource manager needs to load beforehand
        //! order matters!
        void Create(hostRes::HostResources& hostRes, Commands& commands)
        { 
            //? host to vk resources
            ui_fontImages.Create(hostRes.fontTextures, commands.cmdPool);

            //? "factories"
            CreateUniformBuffer_Text    (ui_ubo_array);
            CreateShader_Text           (ui_shader, ui_fontImages);
            CreateRenderPass_Text       (ui_renderPass);
            CreatePipeline_Text         (ui_pipeline, ui_shader, ui_renderPass, ui_ubo_array);
        }

    };

    inline VkResources resources;

}//ns