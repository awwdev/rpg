//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Context.hpp"

namespace rpg::gpu::vuk {

struct Pipeline
{
    VkPipeline pipeline;
    VkPipelineLayout layout;

    void Destroy() 
    {
        vkDestroyPipeline(g_contextPtr->device, pipeline, nullptr);
        vkDestroyPipelineLayout(g_contextPtr->device, layout, nullptr);
    }
    ~Pipeline() { Destroy(); }
};

}//ns