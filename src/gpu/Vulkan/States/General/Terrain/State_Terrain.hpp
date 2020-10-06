//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/States/General/General_RenderPass.hpp"
#include "gpu/Vulkan/States/General/General_Pipeline.hpp"
#include "gpu/Vulkan/States/General/General_Shader.hpp"
#include "gpu/Vulkan/States/General/General_Vertices.hpp"
#include "gpu/Vulkan/States/General/General_Uniforms.hpp"

#include "gpu/Vulkan/States/General/Terrain/Terrain_Shader.hpp"
#include "gpu/Vulkan/States/General/Terrain/Terrain_Pipeline.hpp"
#include "gpu/Vulkan/States/General/Terrain/Terrain_Wire_Shader.hpp"
#include "gpu/Vulkan/States/General/Terrain/Terrain_Wire_Pipeline.hpp"

#include "gpu/RenderData/RenderData.hpp"
#include "res/_Old/CpuResources.hpp"
#include "res/Resources.hpp"

namespace rpg::gpu::vuk {

struct State_Terrain
{
    Terrain_Shader        shader;
    Terrain_Pipeline      pipeline;
    Terrain_Wire_Shader   wireShader;
    Terrain_Wire_Pipeline wirePipeline;

    void Create(General_RenderPass& generalRenderPass, General_Vertices& generalVertices, General_Uniforms& generalUniforms)
    {
        shader       .Create();
        pipeline     .Create(generalRenderPass, shader, generalVertices, generalUniforms);
        wireShader   .Create();
        wirePipeline .Create(generalRenderPass, wireShader, generalVertices, generalUniforms);
    }

    void Destroy()
    {
        pipeline     .Destroy();
        shader       .Destroy();
        wirePipeline .Destroy();
        wireShader   .Destroy();
    }    
};

}//NS