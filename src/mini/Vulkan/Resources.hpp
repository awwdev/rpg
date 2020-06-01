//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"

#include "mini/Vulkan/Commands.hpp"
#include "mini/Vulkan/Synchronization.hpp"
#include "mini/Memory/Allocator.hpp"

#include "mini/Vulkan/Dedicated/Default_RenderPass.hpp"
#include "mini/Vulkan/Dedicated/Default_Pipeline.hpp"
#include "mini/Vulkan/Dedicated/Default_Shader.hpp"

#include "mini/Resources/HostResources.hpp"

namespace mini::vk
{
    struct VkResources
    {
        //? gpu resources
        Image images [hostRes::TextureName::ENUM_END];

        //? pipeline stuff
        Default_RenderPass  default_renderPass;
        Default_Pipeline    default_pipeline;
        Shader              default_shader;

        explicit VkResources(VkDevice pDevice) : default_shader { pDevice } {;}

        //! resource manager needs to load beforehand
        inline void Create(Context& context, hostRes::HostResources& resManager, Commands& commands)
        { 
            //? auto host resources load (based on enum)
            FOR_CARRAY(images, i)
                images[i].Create(context, *resManager.textures.iTextures[i], commands.cmdPool);

            //? factories
            CreateShader_Default(context, default_shader, images);

            default_renderPass.Create(context);
            default_pipeline.Create(context, default_shader, default_renderPass);
        }
        
    };

}//ns