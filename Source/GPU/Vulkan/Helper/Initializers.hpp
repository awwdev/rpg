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

inline auto Rasterization(
const VkCullModeFlags cullmode    = VK_CULL_MODE_BACK_BIT,
const VkPolygonMode   polygonMode = VK_POLYGON_MODE_FILL,
const VkFrontFace     frontFace   = VK_FRONT_FACE_CLOCKWISE)
{
    return VkPipelineRasterizationStateCreateInfo {
        .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .pNext                   = nullptr,
        .flags                   = 0,
        .depthClampEnable        = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode             = polygonMode,
        .cullMode                = cullmode,
        .frontFace               = frontFace,
        .depthBiasEnable         = VK_FALSE,
        .depthBiasConstantFactor = 0.f,
        .depthBiasClamp          = 0.f,
        .depthBiasSlopeFactor    = 0.f,
        .lineWidth               = 1.f  
    };
}

inline auto Multisampling(
const VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT)
{
    return VkPipelineMultisampleStateCreateInfo {
        .sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .pNext                 = nullptr,
        .flags                 = 0,
        .rasterizationSamples  = sampleCount,
        .sampleShadingEnable   = VK_FALSE,
        .minSampleShading      = 1.f,
        .pSampleMask           = nullptr,
        .alphaToCoverageEnable = VK_FALSE,
        .alphaToOneEnable      = VK_FALSE
    };
}

inline auto DepthStencil(
const VkBool32 dephTestEnable = VK_FALSE, const VkBool32 depthWriteEnable = VK_FALSE)
{
    return VkPipelineDepthStencilStateCreateInfo {
        .sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .pNext                 = nullptr,
        .flags                 = 0,
        .depthTestEnable       = dephTestEnable,
        .depthWriteEnable      = depthWriteEnable,
        .depthCompareOp        = VK_COMPARE_OP_LESS,//reversed z
        .depthBoundsTestEnable = VK_FALSE,
        .stencilTestEnable     = VK_FALSE,
        .front                 = {},
        .back                  = {},
        .minDepthBounds        = 0.f,
        .maxDepthBounds        = 1.f
    };
}

inline auto BlendAttachment(const VkBool32 enable)
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

inline auto InputAssemblyDefault(const VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
{
    return VkPipelineInputAssemblyStateCreateInfo {
        .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext                  = nullptr,
        .flags                  = 0,
        .topology               = topology,
        .primitiveRestartEnable = VK_FALSE 
    };
}

inline auto VertexInputInfoEmpty() 
{
    return VkPipelineVertexInputStateCreateInfo {
        .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext                           = nullptr,
        .flags                           = 0,
        .vertexBindingDescriptionCount   = 0,
        .pVertexBindingDescriptions      = nullptr,
        .vertexAttributeDescriptionCount = 0,
        .pVertexAttributeDescriptions    = nullptr
    };
}

template<auto BINDING_COUNT, auto ATTRIBUTE_COUNT>
inline auto VertexInputInfo(
const VkVertexInputBindingDescription   (&bindings)  [BINDING_COUNT],
const VkVertexInputAttributeDescription (&attribues) [ATTRIBUTE_COUNT]) 
{
    return VkPipelineVertexInputStateCreateInfo {
        .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext                           = nullptr,
        .flags                           = 0,
        .vertexBindingDescriptionCount   = BINDING_COUNT,
        .pVertexBindingDescriptions      = bindings,
        .vertexAttributeDescriptionCount = ATTRIBUTE_COUNT,
        .pVertexAttributeDescriptions    = attribues
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

inline auto DescSetLayoutInfo(
const uint32_t bindingCount, VkDescriptorSetLayoutBinding* bindings)
{
     return VkDescriptorSetLayoutCreateInfo {
        .sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext          = nullptr,
        .flags          = 0,
        .bindingCount   = bindingCount,
        .pBindings      = bindings
    };
}


inline auto DescPoolInfo(
const uint32_t maxSets, 
const uint32_t poolSizeCount, VkDescriptorPoolSize* poolSizes
)
{
    return VkDescriptorPoolCreateInfo {
        .sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .pNext          = nullptr,
        .flags          = 0,
        .maxSets        = maxSets,
        .poolSizeCount  = poolSizeCount,
        .pPoolSizes     = poolSizes
    };
}

inline auto DescSetAllocInfo(
VkDescriptorPool descPool,
const uint32_t descSetLayoutCount, VkDescriptorSetLayout* descSetLayouts)
{
    return VkDescriptorSetAllocateInfo {
        .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext              = nullptr,
        .descriptorPool     = descPool,
        .descriptorSetCount = descSetLayoutCount,
        .pSetLayouts        = descSetLayouts
    };
}

inline auto CmdBufferAllocInfo(
VkCommandPool cmdPool, const uint32_t count, 
const VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY)
{
    return VkCommandBufferAllocateInfo {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext              = nullptr,
        .commandPool        = cmdPool,
        .level              = level,
        .commandBufferCount = count
    };
}

inline auto CmdPoolInfo(const uint32_t queueIdx)
{
    return VkCommandPoolCreateInfo {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = queueIdx
    };
}

}//ns