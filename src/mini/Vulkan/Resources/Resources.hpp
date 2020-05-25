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

#include "mini/Memory/Allocator.hpp"
#include "mini/Resources/ResourceManager.hpp"


namespace mini::vk
{
    struct Resources
    {
        //? meta
        Commands            commands;
        Synchronization     synchronization;

        //? pipeline objects
        Default_Shader      default_shader;
        Default_RenderPass  default_renderPass;
        Default_Pipeline    default_pipeline;

        //? resource objects
        Images images;
        
        //assumption: resource manager has loaded beforehand
        inline void Create(Context& context, res::ResourceManager& resManager)
        { 
            //? meta
            commands.Create(context);
            synchronization.Create(context);

            //? resource objects
            images.Load(context, resManager, commands.cmdPool);

            //? pipeline objects
            default_shader.Create(context, images);
            default_renderPass.Create(context);
            default_pipeline.Create(context, default_shader, default_renderPass);
        }
        
    };

}//ns