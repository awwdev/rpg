//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"

#include "mini/Vulkan/Shaders/Default_Shader.hpp"
#include "mini/Vulkan/RenderPasses/Default_RenderPass.hpp"
#include "mini/Vulkan/Pipelines/Default_Pipeline.hpp"
#include "mini/Vulkan/Commands.hpp"
#include "mini/Vulkan/Synchronization.hpp"

//strategy: dedicated structs with dtor (almost "static" resources)(watch multiple dtor call)

namespace mini::vk
{
    struct Resources
    {
        Default_Shader      default_shader;
        Default_RenderPass  default_renderPass;
        Default_Pipeline    default_pipeline;
        Default_Commands    default_commands;
        Default_Sync        default_sync;

        explicit Resources(Context& context)
        { 
            default_shader.Create(context);
            default_renderPass.Create(context);
            default_pipeline.Create(context, default_shader, default_renderPass);
            default_commands.Create(context);
            default_sync.Create(context);

        }
    };

}//ns