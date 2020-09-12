//https://github.com/awwdev

#pragma once
#include "Vulkan/Core.hpp"
#include "Vulkan/Context.hpp"
#include "Vulkan/Resources/Resources_Default.hpp"

#include "Vulkan/Factories/Post/Post_RenderPass.hpp"
#include "Vulkan/Factories/Post/Post_Pipeline.hpp"
#include "Vulkan/Factories/Post/Post_Shader.hpp"

namespace rpg::vk {

struct Resources_Post
{
    RenderPass       renderPass;
    Shader           shader;
    Pipeline         pipeline;

    void Create(VkCommandPool cmdPool, vk::Resources_Common& resCom)
    {
        Post_CreateShader           (shader, resCom.renderPass.renderImage.t);
        Post_CreateRenderPass       (renderPass);
        Post_CreatePipeline         (pipeline, shader, renderPass);
    }

    void Recreate(vk::Resources_Common& resCom)
    {
        renderPass.Clear();
        pipeline.Clear();
        shader.Clear();
        Post_CreateShader           (shader, resCom.renderPass.renderImage.t);
        Post_CreateRenderPass       (renderPass);
        Post_CreatePipeline         (pipeline, shader, renderPass);
    }
};

}//NS