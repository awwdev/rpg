//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/_Old/Objects/Shader.hpp"
#include "GPU/Vulkan/_Old/Objects/UniformBuffer.hpp"
#include "GPU/Vulkan/_Old/Objects/ImageArray.hpp"
#include "Resources/CpuResources.hpp"
#include "Common/Structs.hpp"

namespace rpg::gpu::vuk {

inline void Sky_CreateShader(Shader& shader)
{  
    shader.CreateShaderModule("AssetsApp/Shaders/spv/sky.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shader.CreateShaderModule("AssetsApp/Shaders/spv/sky.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
}

}//ns