//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/States/General/Foliage/Foliage_Shader.hpp"
#include "gpu/Vulkan/States/General/Foliage/Foliage_Pipeline.hpp"

namespace rpg::gpu::vuk {

struct State_Foliage
{
    Foliage_Shader   shader;
    Foliage_Pipeline pipeline;

    void Create(General_RenderPass& generalRenderPass, General_Vertices& generalVertices, General_Uniforms& generalUniforms)
    {
        shader       .Create();
        pipeline     .Create(generalRenderPass, shader, generalVertices, generalUniforms);
    }

    void Destroy()
    {
        pipeline     .Destroy();
        shader       .Destroy();
    }
};

}//NS