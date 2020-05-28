//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"

#include "mini/Vulkan/Default/Default_Shader.hpp"
#include "mini/Vulkan/Default/Default_RenderPass.hpp"
#include "mini/Vulkan/Default/Default_Pipeline.hpp"

#include "mini/Vulkan/Objects/Shader.hpp"

#include "mini/Vulkan/Commands.hpp"
#include "mini/Vulkan/Synchronization.hpp"
#include "mini/Memory/Allocator.hpp"

#include "mini/Resources/ResourceManager.hpp"

namespace mini::vk
{
    struct Resources
    {
        //? resources
        Image images [res::ENUM_END];

        //? pipeline
        //Default_Shader      default_shader;
        Default_RenderPass  default_renderPass;
        Default_Pipeline    default_pipeline;

        Shader shader_default;

        explicit Resources(VkDevice pDevice) : shader_default { pDevice } {;}

        //! resource manager needs to load beforehand
        inline void Create(Context& context, res::ResourceManager& resManager, Commands& commands)
        { 
            //? resources
            FOR_CARRAY(images, i)
                images[i].Create(context, *resManager.textures.mapping[i], commands.cmdPool);

            CreateShader_Default(context, shader_default, images);

            //default_shader.Create(context, images);
            default_renderPass.Create(context);
            default_pipeline.Create(context, shader_default, default_renderPass);
        }
        
    };

}//ns