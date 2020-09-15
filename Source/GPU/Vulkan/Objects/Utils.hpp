//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Objects/Initializers.hpp"
#include "GPU/Vulkan/Objects/Utils.hpp"

#include "Common/Memory/Allocator.hpp"
#include <fstream>

namespace rpg::gpu::vuk {

template<auto BUFFER_SIZE = 8000u>
void CreateShaderModule(
chars_t path, const VkShaderStageFlagBits stage,
VkShaderModule& mod, VkPipelineShaderStageCreateInfo& stageInfo)
{
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    rpg::dbg::Assert(file.is_open(), "cannot open shader file");

    const uint32_t size = (uint32_t)file.tellg();
    auto ptrBuffer = com::mem::ClaimBlock<char[BUFFER_SIZE]>();
    //char buffer [BUFFER_SIZE];
    file.seekg(std::ios::beg);
    file.read(*ptrBuffer, size);
    //file.read(buffer, size);

    const VkShaderModuleCreateInfo moduleInfo {
        .sType      = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext      = nullptr,
        .flags      = 0,
        .codeSize   = size,
        .pCode      = reinterpret_cast<const uint32_t*>(*ptrBuffer)
        //.pCode      = reinterpret_cast<const uint32_t*>(buffer)
    };
    VkCheck(vkCreateShaderModule(g_contextPtr->device, &moduleInfo, nullptr, &mod));

    stageInfo = {
        .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext  = nullptr,
        .flags  = 0,
        .stage  = stage,
        .module = *mod,
        .pName  = "main",
        .pSpecializationInfo = nullptr 
    };
}

}//ns