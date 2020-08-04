//https://github.com/awwdev

#pragma once
#include "mini/Debug/Assert.hpp"
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Ctors.hpp"
#include "mini/Vulkan/Objects/Image.hpp"
#include "mini/Vulkan/Objects/UniformBuffer.hpp"

#include "mini/Memory/Allocator.hpp"
#include "mini/Box/Array.hpp"

namespace mini::vk
{
    struct Shader
    {
        box::Array<VkShaderModule, 4> modules;
        box::Array<VkPipelineShaderStageCreateInfo, 4> stageInfos;

        //uniform data
        UniformInfo info; //! probably need array?
        box::Array<VkSampler, 4> samplers; //just some capacity

        ~Shader()
        {
            FOR_ARRAY(modules, i)    vkDestroyShaderModule  (g_contextPtr->device, modules[i], nullptr);
            FOR_ARRAY(samplers, i)   vkDestroySampler       (g_contextPtr->device, samplers[i], nullptr);
            modules.Clear(); //clear due to dtor call on recreation
            samplers.Clear();
            stageInfos.Clear();
        }

        template<auto BUFFER_SIZE = 8000u>
        void CreateShaderModule(chars_t path, const VkShaderStageFlagBits stage)
        {
            std::ifstream file(path, std::ios::ate | std::ios::binary);
            mini::Assert(file.is_open(), "cannot open shader file");

            const uint32_t size = (uint32_t)file.tellg();
            auto ptrBuffer = mem::ClaimBlock<char[BUFFER_SIZE]>();
            file.seekg(std::ios::beg);
            file.read(*ptrBuffer, size);

            const VkShaderModuleCreateInfo moduleInfo {
                .sType      = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
                .pNext      = nullptr,
                .flags      = 0,
                .codeSize   = size,
                .pCode      = reinterpret_cast<const uint32_t*>(*ptrBuffer)
            };

            auto* mod = modules.Append();
            VK_CHECK(vkCreateShaderModule(g_contextPtr->device, &moduleInfo, nullptr, mod));

            stageInfos.Append(VkPipelineShaderStageCreateInfo{
                .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .pNext  = nullptr,
                .flags  = 0,
                .stage  = stage,
                .module = *mod,
                .pName  = "main",
                .pSpecializationInfo = nullptr 
            });
        }

    };

}//ns