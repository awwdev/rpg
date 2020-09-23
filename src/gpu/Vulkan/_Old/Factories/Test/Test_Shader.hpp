
//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/_Old/Objects/Shader.hpp"

namespace rpg::gpu::vuk {
    
inline void Test_CreateShader(Shader& shader)
{  
    shader.CreateShaderModule("res/Shaders/spv/test.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shader.CreateShaderModule("res/Shaders/spv/test.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
}

}//NS