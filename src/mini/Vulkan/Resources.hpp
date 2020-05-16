#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"

#include "mini/Vulkan/Shader.hpp"
#include "mini/Vulkan/RenderPass.hpp"
#include "mini/Vulkan/Pipeline.hpp"


namespace mini::vk
{
    struct Resources
    {
        //? DEFAULT
        Shader default_shader;
        //RenderPass  renderPass_default;
        //Pipeline    pipeline_default;

        explicit Resources(Context& context)
             : default_shader { context.device, "res/default.vert.spv", "res/default.frag.spv" }
        { ; }

    };

}//ns