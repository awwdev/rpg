//https://github.com/awwdev

#pragma once
#include "Vulkan/Objects/Shader.hpp"
#include "Vulkan/Objects/UniformBuffer.hpp"
#include "Vulkan/Objects/ImageArray.hpp"
#include "Resources/CpuResources.hpp"
#include "Common/Structs.hpp"

namespace rpg::vuk {

inline void Sky_CreateShader(Shader& shader)
{  
    shader.CreateShaderModule("res/Shaders/spv/sky.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shader.CreateShaderModule("res/Shaders/spv/sky.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
}

}//ns