
//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Objects/Shader.hpp"

namespace rpg::gpu::vuk {
    
inline void Test_CreateShader(Shader& shader)
{  
    shader.CreateShaderModule("AssetsApp/Shaders/spv/test.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shader.CreateShaderModule("AssetsApp/Shaders/spv/test.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
}

}//NS