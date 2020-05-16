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
        Context& context;

        Shader default_shader;
        //RenderPass  renderPass_default;
        //Pipeline    pipeline_default;

        explicit Resources(Context& pContext)
             : context          { pContext }
             , default_shader   { context.device, "res/default.vert.spv", "res/default.frag.spv" }
        {
        }

    };

    //raii (dtor/ctor and refs) or dedicated create/destroy functions (passing needed stuff, more pure)
    //oop seems more convenient here since stuff is coupled anyways?
    //now it gets hairy when there is a lot of stuff like in Context ...

}//ns