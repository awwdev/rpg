#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"

#include "mini/Vulkan/Shader.hpp"
#include "mini/Vulkan/RenderPass.hpp"
#include "mini/Vulkan/Pipeline.hpp"

//strategy: dedicated structs with dtor (almost "static" resources)(watch multiple dtor call)

namespace mini::vk
{
    struct Resources
    {
        Default_Shader      default_shader;
        Default_RenderPass  default_renderPass;
        Default_Pipeline    default_pipeline;


        explicit Resources(Context& context)
            : default_shader      { context }
            , default_renderPass  { context }
            , default_pipeline    { context, default_shader, default_renderPass }
        { ; }
    };

}//ns