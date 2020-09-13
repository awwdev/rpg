//https://github.com/awwdev

#pragma once
#include "Vulkan/Core.hpp"
#include "Vulkan/Context.hpp"
#include "Vulkan/Resources/Resources_Test.hpp"

#include "Vulkan/Factories/Post/Post_RenderPass.hpp"
#include "Vulkan/Factories/Post/Post_Pipeline.hpp"
#include "Vulkan/Factories/Post/Post_Shader.hpp"

namespace rpg::vk {

struct Resources_Post
{
    RenderPass       renderPass;
    Shader           shader;
    Pipeline         pipeline;

    void Create(vk::Resources_Test& test)
    {
        Post_CreateShader           (shader, test.renderPass.renderImage);
        Post_CreateRenderPass       (renderPass);
        Post_CreatePipeline         (pipeline, shader, renderPass);
    }

    void Recreate(vk::Resources_Test& test)
    {
        renderPass.Clear();
        pipeline.Clear();
        shader.Clear();
        Post_CreateShader           (shader, test.renderPass.renderImage);
        Post_CreateRenderPass       (renderPass);
        Post_CreatePipeline         (pipeline, shader, renderPass);
    }
};

}//NS