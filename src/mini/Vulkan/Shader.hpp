#pragma once
#include "mini/Debug/Assert.hpp"
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Ctors.hpp"

#include <fstream>

namespace mini::vk
{
    struct Shader
    {
        VkDevice& device;

        //module handles are inside stages info
        VkPipelineShaderStageCreateInfo stages [2];


        Shader(VkDevice pDevice, chars_t vertPath, chars_t fragPath)
             : device { pDevice }
             , stages { 
                ctor::CreateShaderStage(nullptr, VK_SHADER_STAGE_VERTEX_BIT),
                ctor::CreateShaderStage(nullptr, VK_SHADER_STAGE_FRAGMENT_BIT)
            }
        {
            const auto createModule = [&](chars_t path, VkShaderModule* mod) //ref is important!
            {
                std::ifstream file(path, std::ios::ate | std::ios::binary);
                mini::Assert(file.is_open(), "cannot open shader file");

                const uint32_t size = file.tellg();
                char vec[10000]; //!be aware of capacity
                file.seekg(0);
                file.read(vec, size);

                const VkShaderModuleCreateInfo moduleInfo {
                    .sType      = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
                    .pNext      = nullptr,
                    .flags      = 0,
                    .codeSize   = size,
                    .pCode      = reinterpret_cast<const uint32_t*>(vec)
                };
                VK_CHECK(vkCreateShaderModule(device, &moduleInfo, nullptr, mod)); //watch ptr here
            };

            //FOR_CARRAY(stages, i) createModule(vertPath, &(stages[i].module));
            createModule(vertPath, &(stages[0].module));
            //vkDestroyShaderModule(device, stages[0].module, nullptr);
            //FOR_CARRAY(stages, i) vkDestroyShaderModule(device, stages[i].module, nullptr);
        }

        ~Shader()
        {
            //! crash ?!?!
            //FOR_CARRAY(stages, i) 
            //vkDestroyShaderModule(device, stages[0].module, nullptr);
        }

    };

}//ns