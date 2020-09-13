
//https://github.com/awwdev

#pragma once
#include "Vulkan/Objects/Shader.hpp"

namespace rpg::vk {
    
inline void Test_CreateShader(Shader& shader)
{  
    shader.CreateShaderModule("res/Shaders/spv/test.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shader.CreateShaderModule("res/Shaders/spv/test.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
}

}//NS