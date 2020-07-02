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

#include "mini/Memory/Allocator.hpp"
#include "mini/Resources/HostResources.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/RenderGraph/IRenderer.hpp"
#include "mini/RenderGraph/UboData.hpp"

namespace mini::vk
{
    struct VkResources
    {
        //? host to vk resources
        ImageArray text_imageArray;

        //? vk pipeline resources
        Default_PushConstants   text_pushConst;
        RenderPass              text_renderPass;
        Shader                  text_shader;
        Pipeline                text_pipeline;

        UniformBuffer_Array<rendergraph::UniformData_Text, 1000> text_ubo_array;


        explicit VkResources(Context& context) 
            : text_pushConst    {}
            , text_pipeline     { context }
            , text_shader       { context }
            , text_renderPass   { context }
            , text_ubo_array    {}
        {;}

        //! resource manager needs to load beforehand
        //! order matters!
        inline void Create(Context& context, hostRes::HostResources& hostRes, Commands& commands)
        { 
            //? host to vk resources
            text_imageArray.Create(context, hostRes.textureArray, commands.cmdPool);

            //? "factories"
            CreateUniformBuffer_Text    (context, text_ubo_array);
            CreateShader_Text           (context, text_shader, text_imageArray);
            CreateRenderPass_Text       (context, text_renderPass);
            CreatePipeline_Text         (context, text_pipeline, text_shader, text_renderPass, text_ubo_array);
        }

    };

}//ns