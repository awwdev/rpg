#pragma once
#include "mini/Debug/Assert.hpp"
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Ctors.hpp"

#include <fstream>


namespace mini::vk
{
    inline VkPipelineShaderStageCreateInfo CreateStageInfo (const VkShaderStageFlagBits stage)
    {
        return {
            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext  = nullptr,
            .flags  = 0,
            .stage  = stage,
            .module = nullptr, //filled in by CreateShaderModule()
            .pName  = "main",
            .pSpecializationInfo = nullptr 
        };
    }

    inline VkShaderModule CreateShaderModule(VkDevice device, chars_t path)
    {
        std::ifstream file(path, std::ios::ate | std::ios::binary);
        mini::Assert(file.is_open(), "cannot open shader file");

        const uint32_t size = file.tellg();
        char buffer[10000]; //!be aware of capacity, maybe use allocator to not exhaust stack
        file.seekg(0);
        file.read(buffer, size);

        const VkShaderModuleCreateInfo moduleInfo {
            .sType      = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext      = nullptr,
            .flags      = 0,
            .codeSize   = size,
            .pCode      = reinterpret_cast<const uint32_t*>(buffer)
        };

        VkShaderModule mod;
        VK_CHECK(vkCreateShaderModule(device, &moduleInfo, nullptr, &mod));
        return mod;
    }


    //? DEDICATED STRUCTS

    struct Shader_default
    {
        VkDevice device;

        const VkVertexInputBindingDescription BINDING_DESCS [2] 
        {
            {
                .binding    = 0,
                .stride     = sizeof(float), //!fill vertex
                .inputRate  = VK_VERTEX_INPUT_RATE_VERTEX
            },
        };

        const VkVertexInputAttributeDescription ATTRIBUTE_DESCS [2]
        {
            { //? position
                .location   = 0,
                .binding    = 0,
                .format     = VK_FORMAT_R32G32_SFLOAT,
                .offset     = 0, //!offset
            },
            { //? color
                .location   = 1,
                .binding    = 0,
                .format     = VK_FORMAT_R32G32B32A32_SFLOAT,
                .offset     = sizeof(float), //!offset
            }
        };

        VkPipelineShaderStageCreateInfo stages [2] //module will be filled in (ctor)
        {
            CreateStageInfo(VK_SHADER_STAGE_VERTEX_BIT),
            CreateStageInfo(VK_SHADER_STAGE_FRAGMENT_BIT)
        };


        inline void LoadShader()
        {
            stages[0].module = CreateShaderModule(device, "res/default.vert.spv");
            stages[1].module = CreateShaderModule(device, "res/default.frag.spv");
        }

        ~Shader_default()
        {
            //!make sure device was set
            FOR_CARRAY(stages, i)
                vkDestroyShaderModule(device, stages[i].module, nullptr);
        }
    };

}//ns