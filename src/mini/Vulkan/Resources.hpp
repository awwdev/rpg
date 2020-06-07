//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"

#include "mini/Vulkan/Commands.hpp"
#include "mini/Vulkan/Synchronization.hpp"

#include "mini/Vulkan/Dedicated/Default_RenderPass.hpp"
#include "mini/Vulkan/Dedicated/Default_Pipeline.hpp"
#include "mini/Vulkan/Dedicated/Default_Shader.hpp"
#include "mini/Vulkan/Dedicated/Default_VertexBuffer.hpp"
#include "mini/Vulkan/Dedicated/Default_UniformBuffer.hpp"
#include "mini/Vulkan/Objects/PushConstants.hpp"

#include "mini/Memory/Allocator.hpp"
#include "mini/Resources/HostResources.hpp"
#include "mini/Utils/Vertex.hpp"

namespace mini::vk
{
    struct VkResources
    {
        //? gpu resources
        Image images [hostRes::TextureName::ENUM_END];

        //? pipeline stuff
        //TODO: rename to UI (instead of default_)
        Default_RenderPass      default_renderPass;
        Default_Pipeline        default_pipeline;
        Default_PushConstants   default_pc;

        Shader                  default_shader;
        VertexBuffer            default_vb;
        UniformBuffer           default_ub;

        //TODO: instead of bare numbers for each buffer, something like ENTITY_COUNT would be nice
        //however entities seldom have same vert cound, iterating entities (mehses) and collecting all needed verts and then rough estimate of count ...
        explicit VkResources(Context& context) 
            : default_shader { context.device } 
            , default_vb     { 10000 }
            , default_ub     { context, 100 } //!size explodes due to min alignment

        {;}

        //! resource manager needs to load beforehand
        //! order matters!
        inline void Create(Context& context, hostRes::HostResources& resManager, Commands& commands)
        { 
            //? auto host resources load (based on enum)
            FOR_CARRAY(images, i)
                images[i].Create(context, *resManager.textures.iTextures[i], commands.cmdPool);

            //? "factories"
            default_ub.Create(context); //maybe no "factory method" needed
            CreateVertexBuffer_Default(context, default_vb);
            CreateShader_Default(context, default_shader, images, default_ub); //or pass some upper struct UBOs ?

            default_renderPass.Create(context);
            default_pipeline.Create(
                context, 
                default_shader, 
                default_renderPass, 
                CreatePipelineVertexInputInfo(default_vb)
            );
        }

    };

}//ns