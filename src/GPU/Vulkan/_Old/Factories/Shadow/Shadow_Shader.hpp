//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Objects/Shader.hpp"
#include "GPU/Vulkan/Objects/UniformBuffer.hpp"
#include "GPU/Vulkan/Objects/ImageArray.hpp"
#include "Resources/CpuResources.hpp"
#include "Common/Structs.hpp"
#include "GPU/Vulkan/_Old/Factories/Shadow/Shadow_RenderPass.hpp"

namespace rpg::gpu::vuk {

inline void Shadow_CreateShader(Shader& shader, RenderPassDepth&)
{  
    shader.CreateShaderModule("res/Shaders/spv/shadow.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shader.CreateShaderModule("res/Shaders/spv/shadow.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
}

}//ns