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
#include "mini/Utils/Vertex.hpp"

namespace mini::vk
{
    struct VkResources
    {
        Image images [hostRes::TextureName::ENUM_END];

        //? pipeline stuff
        RenderPass              ui_renderPass;
        Pipeline                ui_pipeline;
        Pipeline                ui_pipeline_wire;
        Default_PushConstants   ui_pushConst;
        Shader                  ui_shader;
        Shader                  ui_shader_wire;
        VertexBuffer            ui_vbo;
        UniformBuffer           ui_ibo;

        //TODO: instead of bare numbers for each buffer, something like ENTITY_COUNT would be nice
        //however entities seldom have same vert count, iterating entities (mehses) and collecting all needed verts and then rough estimate of count ...
        explicit VkResources(Context& context) 
            : ui_pipeline       { context }
            , ui_pipeline_wire  { context }
            , ui_renderPass     { context }
            , ui_shader         { context } 
            , ui_shader_wire    { context } 
            , ui_vbo            { 10000 }
            , ui_ibo            { context, 250 } 
        {;}

        //! resource manager needs to load beforehand
        //! order matters!
        inline void Create(Context& context, hostRes::HostResources& hostRes, Commands& commands)
        { 
            //? auto host resources load (based on enum)
            FOR_CARRAY(images, i)
                images[i].Create(context, *hostRes.textures.iTextures[i], commands.cmdPool);

            //? "factories"
            CreateUniformBuffer_UI(context, ui_ibo); //not really needed but consistent (and maybe future proof)
            CreateVertexBuffer_UI(context, ui_vbo);
            CreateShader_UI(context, ui_shader, images); 
            CreateShader_UI_wire(context, ui_shader_wire, images); 

            CreateRenderPass_UI(context, ui_renderPass);
            CreatePipeline_UI(
                context, 
                ui_pipeline,
                ui_shader, 
                ui_renderPass, 
                ui_vbo, 
                ui_ibo
            );
            CreatePipeline_UI_wire(
                context, 
                ui_pipeline_wire,
                ui_shader_wire, 
                ui_renderPass, 
                ui_vbo, 
                ui_ibo
            );
        }

    };

}//ns