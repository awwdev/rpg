//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/_Old/Objects/Shader.hpp"
#include "gpu/Vulkan/_Old/Objects/UniformBuffer.hpp"
#include "gpu/Vulkan/_Old/Objects/ImageArray.hpp"
#include "res/_Old/CpuResources.hpp"
#include "com/Structs.hpp"

namespace rpg::gpu::vuk {

inline void Sky_CreateShader(Shader& shader)
{  
    shader.CreateShaderModule("res/Shaders/spv/sky.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shader.CreateShaderModule("res/Shaders/spv/sky.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
}

}//ns