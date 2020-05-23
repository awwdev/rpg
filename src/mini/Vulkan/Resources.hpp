//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"

#include "mini/Vulkan/Default/Default_Shader.hpp"
#include "mini/Vulkan/Default/Default_RenderPass.hpp"
#include "mini/Vulkan/Default/Default_Pipeline.hpp"
#include "mini/Vulkan/Commands.hpp"
#include "mini/Vulkan/Synchronization.hpp"
#include "mini/Vulkan/Objects/Image.hpp"

//strategy: dedicated structs with dtor (almost "static" resources)(watch multiple dtor call)

namespace mini::vk
{
    struct Resources
    {
        Default_Shader      default_shader;
        Default_RenderPass  default_renderPass;
        Default_Pipeline    default_pipeline;

        Commands            commands;
        Synchronization     synchronization;

        //other
        Image image_font;


        explicit Resources(Context& context)
        { 
            default_shader.Create(context);
            default_renderPass.Create(context);
            default_pipeline.Create(context, default_shader, default_renderPass);
            
            commands.Create(context);
            synchronization.Create(context);

            image_font.Create(context, 32, 32);

        }
    };

}//ns