//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Objects/Shader.hpp"
#include "mini/Vulkan/Objects/UniformBuffer.hpp"
#include "mini/Vulkan/Objects/ImageArray.hpp"
#include "mini/Resources/HostResources.hpp"
#include "mini/Utils/Structs.hpp"

namespace mini::vk {

inline void Sky_CreateShader(Shader& shader)
{  
    shader.CreateShaderModule("res/Shaders/spv/sky.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shader.CreateShaderModule("res/Shaders/spv/sky.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
}

}//ns