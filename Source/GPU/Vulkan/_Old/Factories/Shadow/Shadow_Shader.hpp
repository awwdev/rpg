//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/_Old/Objects/Shader.hpp"
#include "gpu/Vulkan/_Old/Objects/UniformBuffer.hpp"
#include "gpu/Vulkan/_Old/Objects/ImageArray.hpp"
#include "res/_Old/CpuResources.hpp"
#include "com/Structs.hpp"
#include "gpu/Vulkan/_Old/Factories/Shadow/Shadow_RenderPass.hpp"

namespace rpg::gpu::vuk {

inline void Shadow_CreateShader(Shader& shader, RenderPassDepth&)
{  
    shader.CreateShaderModule("AssetsApp/Shaders/spv/shadow.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shader.CreateShaderModule("AssetsApp/Shaders/spv/shadow.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
}

}//ns