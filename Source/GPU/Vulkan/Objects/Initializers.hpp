//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"

namespace rpg::gpu::vuk {

template<class T>
inline auto Viewport(
const T width, const T height)
{
        return VkViewport {
        .x        = 0.f,
        .y        = 0.f,
        .width    = static_cast<float>(width),
        .height   = static_cast<float>(height),
        .minDepth = 0.f,
        .maxDepth = 1.f
    };
}

template<class T>
inline auto Scissor(
const T width, const T height)
{
    return VkRect2D {
        .offset = VkOffset2D { 0, 0 },
        .extent = VkExtent2D { 
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height) 
        }
    };
}

inline auto ViewportState(
const VkViewport& viewport, const VkRect2D& scissor)
{
    return VkPipelineViewportStateCreateInfo {
        .sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .pNext         = nullptr,
        .flags         = 0,
        .viewportCount = 1, 
        .pViewports    = &viewport,
        .scissorCount  = 1,
        .pScissors     = &scissor
    };
}

inline auto BlendAttachent(
const VkBool32 enable)
{
    return VkPipelineColorBlendAttachmentState {
        .blendEnable                = enable,
        .srcColorBlendFactor        = VK_BLEND_FACTOR_SRC_ALPHA,
        .dstColorBlendFactor        = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, 
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

inline auto BlendState(
const VkPipelineColorBlendAttachmentState& blendAttachment)
{
    return VkPipelineColorBlendStateCreateInfo {
        .sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .pNext           = nullptr,
        .flags           = 0,
        .logicOpEnable   = VK_FALSE,
        .logicOp         = VK_LOGIC_OP_COPY,
        .attachmentCount = 1,
        .pAttachments    = &blendAttachment,
        .blendConstants  = { 0.f, 0.f, 0.f, 0.f }
    };
}

inline auto PipelineLayoutInfo(
VkDescriptorSetLayout* setLayouts, const uint32_t setLayoutsCount,
VkPushConstantRange* pushConsts = nullptr, const uint32_t pushConstCount = 0)
{
    return VkPipelineLayoutCreateInfo {
        .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext                  = nullptr,
        .flags                  = 0,
        .setLayoutCount         = setLayoutsCount,
        .pSetLayouts            = setLayouts,
        .pushConstantRangeCount = pushConstCount,
        .pPushConstantRanges    = pushConsts,
    };
}

inline auto InputAssembly(
const VkPrimitiveTopology topology)
{
    return VkPipelineInputAssemblyStateCreateInfo {
        .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext                  = nullptr,
        .flags                  = 0,
        .topology               = topology,
        .primitiveRestartEnable = VK_FALSE 
    };
}

inline auto CommandBufferBeginInfo(const VkCommandBufferUsageFlags flags = 0)
{
    return VkCommandBufferBeginInfo {
        .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext            = nullptr,
        .flags            = flags,
        .pInheritanceInfo = nullptr
    };
}

}//ns