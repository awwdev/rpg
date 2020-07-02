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
#include "mini/Box/Map.hpp"

namespace mini::vk
{
    struct Shader
    {
        VkDevice device;

        box::Array<VkShaderModule, 4> modules;
        box::Array<VkPipelineShaderStageCreateInfo, 4> stageInfos;

        //uniform data
        UniformInfo info; //! probably need array?
        box::Array<VkSampler, 4> samplers; //just some capacity

        
        explicit Shader(Context& context) : device { context.device } {;}

        ~Shader()
        {
            FOR_ARRAY(modules, i)    vkDestroyShaderModule(device, modules[i], nullptr);
            FOR_ARRAY(samplers, i)   vkDestroySampler(device, samplers[i], nullptr);
        }

        template<auto BUFFER_SIZE = 10000u>
        void CreateShaderModule(chars_t path, const VkShaderStageFlagBits stage)
        {
            std::ifstream file(path, std::ios::ate | std::ios::binary);
            mini::Assert(file.is_open(), "cannot open shader file");

            const uint32_t size = file.tellg();
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

            auto& mod = modules.AppendReturn();
            VK_CHECK(vkCreateShaderModule(device, &moduleInfo, nullptr, &mod));

            stageInfos.Append(VkPipelineShaderStageCreateInfo{
                .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .pNext  = nullptr,
                .flags  = 0,
                .stage  = stage,
                .module = mod,
                .pName  = "main",
                .pSpecializationInfo = nullptr 
            });
        }

    };

}//ns