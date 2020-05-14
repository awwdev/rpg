#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"

#include "mini/Vulkan/Pipeline.hpp"


namespace mini::vk
{
    struct Resources
    {
        Pipeline pipeline_default;

    };

    inline Resources CreateResources(Context& context)
    {
        Resources resources;

        resources.pipeline_default = CreatePipeline_Default(context);

        return resources;
    }

}//ns