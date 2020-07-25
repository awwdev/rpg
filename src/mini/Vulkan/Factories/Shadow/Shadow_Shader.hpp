//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Objects/Shader.hpp"
#include "mini/Vulkan/Objects/UniformBuffer.hpp"
#include "mini/Vulkan/Objects/ImageArray.hpp"
#include "mini/Resources/HostResources.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/Vulkan/Factories/Shadow/Shadow_RenderPass.hpp"

namespace mini::vk
{
    inline void Shadow_CreateShader(Shader& shader, RenderPassDepth& rp)
    {  
        shader.CreateShaderModule("res/Shaders/spv/shadow.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
        shader.CreateShaderModule("res/Shaders/spv/shadow.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

    }

}//ns