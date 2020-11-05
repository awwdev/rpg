//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Abstraction/Meta/Context.hpp"
#include "com/mem/Allocator.hpp"
#include <fstream>

namespace rpg::gpu::vuk {

enum class ShaderStage //could be moved into a more API agnostic space
{
    Vert,
    Frag,
    Geom,
    Comp,
};

inline constexpr auto GetVulkanShaderStage(ShaderStage const stage)
{
    switch(stage)
    {
        case ShaderStage::Vert: return VK_SHADER_STAGE_VERTEX_BIT;
        case ShaderStage::Frag: return VK_SHADER_STAGE_FRAGMENT_BIT;
        case ShaderStage::Geom: return VK_SHADER_STAGE_GEOMETRY_BIT;
        case ShaderStage::Comp: return VK_SHADER_STAGE_COMPUTE_BIT;
        default: return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
    }
}

template<auto... STAGES_T>
struct Shader
{
    static auto constexpr STAGE_COUNT = static_cast<uint32_t>(sizeof...(STAGES_T));
    static auto constexpr STAGES = { STAGES_T... };
    
    VkShaderModule modules [STAGE_COUNT];
    VkPipelineShaderStageCreateInfo infos [STAGE_COUNT];

    void CreateStage(ShaderStage const stage, chars_t path)
    {
        //open file
        std::ifstream file(path, std::ios::ate | std::ios::binary);
        rpg::dbg::Assert(file.is_open(), "[Shader Assert] cannot open file");

        //read file
        auto constexpr BUFFER_SIZE = 8'000;
        auto const size = (uint32_t)file.tellg();
        struct Arr { char data [BUFFER_SIZE]; };
        auto  ptrBufferArr = com::mem::ClaimBlock<Arr>();
        auto& buffer = ptrBufferArr->data;
        file.seekg(std::ios::beg);
        file.read(buffer, size);

        //? module
        VkShaderModuleCreateInfo const moduleInfo {
            .sType      = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext      = nullptr,
            .flags      = 0,
            .codeSize   = size,
            .pCode      = reinterpret_cast<uint32_t const*>(buffer)
        };
        auto& mod = modules[(idx_t)stage];
        VkCheck(vkCreateShaderModule(g_contextPtr->device, &moduleInfo, nullptr, &mod));

        //? stage info
        auto& info = infos[(idx_t)stage];
        info = {
            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext  = nullptr,
            .flags  = 0,
            .stage  = GetVulkanShaderStage(stage),
            .module = mod,
            .pName  = "main",
            .pSpecializationInfo = nullptr 
        };
    }

    void Destroy() 
    {
        FOR_C_ARRAY(modules, i)
        vkDestroyShaderModule(g_contextPtr->device, modules[i], nullptr);    
        FOR_C_ARRAY(infos, i)
        infos[i] = {};
    }
    ~Shader() { Destroy(); }
};

using VertexFragmentShader = Shader<ShaderStage::Vert, ShaderStage::Frag>;

}//ns