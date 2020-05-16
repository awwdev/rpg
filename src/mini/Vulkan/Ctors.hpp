#pragma once
#include "mini/Vulkan/Core.hpp"

//create default structs with often needed params
//customize struct further after return when necessary

namespace mini::vk::ctor
{
    inline VkViewport viewport(const float width, const float height)
    {
        return {
            .x        = 0.f,
            .y        = 0.f,
            .width    = width,
            .height   = height,
            .minDepth = 0.f,
            .maxDepth = 1.f
        };
    }

    inline VkRect2D scissor(const uint32_t width, const uint32_t height)
    {
        return { 
            .offset = VkOffset2D { 0, 0 },
            .extent = VkExtent2D { width, height }
        };
    }

    inline VkPipelineInputAssemblyStateCreateInfo inputAssembly(const VkPrimitiveTopology topology)
    {
        return {
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .pNext                  = nullptr,
            .flags                  = 0,
            .topology               = topology,
            .primitiveRestartEnable = VK_FALSE 
        };
    }

    inline VkPipelineColorBlendAttachmentState colorBlend(const VkBool32 enable)
    {
        return {
            .blendEnable                = enable,
            .srcColorBlendFactor        = VK_BLEND_FACTOR_SRC_ALPHA, //VK_BLEND_FACTOR_ONE,//VK_BLEND_FACTOR_SRC_ALPHA,
            .dstColorBlendFactor        = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, //VK_BLEND_FACTOR_ZERO,//VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
            .colorBlendOp               = VK_BLEND_OP_ADD,
            .srcAlphaBlendFactor        = VK_BLEND_FACTOR_ONE,
            .dstAlphaBlendFactor        = VK_BLEND_FACTOR_ZERO,
            .alphaBlendOp               = VK_BLEND_OP_ADD,
            .colorWriteMask             = 
                VK_COLOR_COMPONENT_R_BIT | 
                VK_COLOR_COMPONENT_G_BIT | 
                VK_COLOR_COMPONENT_B_BIT | 
                VK_COLOR_COMPONENT_A_BIT
        };
    }

    
    inline VkPipelineShaderStageCreateInfo CreateShaderStage
        (VkShaderModule pModule, VkShaderStageFlagBits stage)
    {
        return{
            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext  = nullptr,
            .flags  = 0,
            .stage  = stage,
            .module = pModule, //VK_SHADER_STAGE_VERTEX_BIT
            .pName  = "main",
            .pSpecializationInfo = nullptr
        };
    }


}//ns