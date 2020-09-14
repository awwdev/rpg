//https://github.com/awwdev

#pragma once
#include "Vulkan/Core.hpp"
#include "Vulkan/Context.hpp"
#include "Vulkan/Resources/Resources_Test.hpp"

#include "Vulkan/Factories/Post/Post_RenderPass.hpp"
#include "Vulkan/Factories/Post/Post_Pipeline.hpp"
#include "Vulkan/Factories/Post/Post_Shader.hpp"
#include "Vulkan/Factories/Post/Post_VertexBuffer.hpp"

namespace rpg::vuk {

struct Resources_Post
{
    RenderPass        renderPass;
    Shader            shader;
    Pipeline          pipeline;
    Post_VertexBuffer vbo;

    void Create(vuk::Resources_Test& test, VkCommandPool cmdPool)
    {
        Post_CreateVertexBuffer     (vbo, cmdPool);
        Post_CreateShader           (shader, test.renderPass.renderImage);
        Post_CreateRenderPass       (renderPass);
        Post_CreatePipeline         (pipeline, shader, renderPass, vbo);
    }

    void Recreate(vuk::Resources_Test& test)
    {
        renderPass.Clear();
        pipeline.Clear();
        shader.Clear();
        Post_CreateShader           (shader, test.renderPass.renderImage);
        Post_CreateRenderPass       (renderPass);
        Post_CreatePipeline         (pipeline, shader, renderPass, vbo);
    }
};

}//NS