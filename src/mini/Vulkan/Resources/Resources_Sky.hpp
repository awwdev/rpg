//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"
#include "mini/Resources/HostResources.hpp"
#include "mini/Vulkan/Resources/Resources_Default.hpp"

#include "mini/Vulkan/Factories/Sky/Sky_Pipeline.hpp"
#include "mini/Vulkan/Factories/Sky/Sky_Shader.hpp" 

namespace mini::vk {

struct Resources_Sky
{
    Shader            shader;
    Pipeline          pipeline;
    Sky_PushConstants pushConsts;


    void Create(VkCommandPool& cmdPool, Resources_Default& default)
    {
        Sky_CreateShader        (shader);
        Sky_CreatePipeline      (pipeline, shader, default.renderPass);
    }

    void Recreate(VkCommandPool& cmdPool, Resources_Default& default)
    {
        shader.~Shader();
        pipeline.~Pipeline();

        Sky_CreateShader        (shader);
        Sky_CreatePipeline      (pipeline, shader, default.renderPass);
    }
};

}//NS