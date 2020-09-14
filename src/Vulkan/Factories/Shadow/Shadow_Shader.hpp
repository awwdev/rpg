//https://github.com/awwdev

#pragma once
#include "Vulkan/Objects/Shader.hpp"
#include "Vulkan/Objects/UniformBuffer.hpp"
#include "Vulkan/Objects/ImageArray.hpp"
#include "Resources/HostResources.hpp"
#include "Common/Structs.hpp"
#include "Vulkan/Factories/Shadow/Shadow_RenderPass.hpp"

namespace rpg::vuk {

inline void Shadow_CreateShader(Shader& shader, RenderPassDepth&)
{  
    shader.CreateShaderModule("res/Shaders/spv/shadow.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shader.CreateShaderModule("res/Shaders/spv/shadow.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
}

}//ns