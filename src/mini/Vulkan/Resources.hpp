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
        Shader_default default_shader;


        explicit Resources(Context& context)
            : default_shader { context.device }
        { 
            default_shader.LoadShader();
        }

    };

}//ns