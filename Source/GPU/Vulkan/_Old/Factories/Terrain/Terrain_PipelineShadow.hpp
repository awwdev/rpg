//https://github.com/awwdev

#pragma once

#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/_Old/Objects/Shader.hpp"
#include "GPU/Vulkan/_Old/Objects/VertexBuffer.hpp"
#include "GPU/Vulkan/_Old/Objects/UniformBuffer.hpp"
#include "GPU/Vulkan/_Old/Objects/PushConstants.hpp"
#include "GPU/Vulkan/_Old/Objects/Pipeline.hpp"
#include "GPU/Vulkan/_Old/Objects/RenderPass.hpp"
#include "GPU/RenderData.hpp"

namespace rpg::gpu::vuk {

inline void Terrain_CreatePipelineShadow(
    Pipeline& pipeline,
    Shader& shader, 
    RenderPassDepth& renderPass,
    VertexBufferOld<com::Common_Vertex, gpu::TERRAIN_VERTEX_MAX_COUNT>& vbo)
{
    const auto vertexInput   = VertexInputInfo();
    const auto inputAssembly = CreatePipelineInputAssemblyInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

    const VkViewport viewport {
        .x        = 0.f,
        .y        = 0.f,
        .width    = (f32)renderPass.width,
        .height   = (f32)renderPass.height,
        .minDepth = 0.f,
        .maxDepth = 1.f
    };

    const VkRect2D scissor { 
        .offset = VkOffset2D { 0, 0 },
        .extent = VkExtent2D 
        { 
            renderPass.width, 
            renderPass.height 
        }
    };

    const VkPipelineViewportStateCreateInfo viewportState {
        .sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .pNext         = nullptr,
        .flags         = 0,
        .viewportCount = 1, 
        .pViewports    = &viewport,
        .scissorCount  = 1,
        .pScissors     = &scissor
    };

    const VkPipelineRasterizationStateCreateInfo rasterization {
        .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .pNext                   = nullptr,
        .flags                   = 0,
        .depthClampEnable        = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode             = VK_POLYGON_MODE_FILL,
        .cullMode                = VK_CULL_MODE_NONE, //VK_CULL_MODE_BACK_BIT,
        .frontFace               = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable         = VK_TRUE,
        .depthBiasConstantFactor = -2000.f,
        .depthBiasClamp          =  0.f,
        .depthBiasSlopeFactor    = -0.5f,
        .lineWidth               =  1.f  
    };

    const VkPipelineMultisampleStateCreateInfo multisampling {
        .sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .pNext                 = nullptr,
        .flags                 = 0,
        .rasterizationSamples  = renderPass.sampleCount,
        .sampleShadingEnable   = VK_FALSE,
        .minSampleShading      = 1.f,
        .pSampleMask           = nullptr,
        .alphaToCoverageEnable = VK_FALSE,
        .alphaToOneEnable      = VK_FALSE
    };

    const VkPipelineDepthStencilStateCreateInfo depthStencil {
        .sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .pNext                 = nullptr,
        .flags                 = 0,
        .depthTestEnable       = VK_TRUE,
        .depthWriteEnable      = VK_TRUE,
        .depthCompareOp        = VK_COMPARE_OP_GREATER_OR_EQUAL,
        .depthBoundsTestEnable = VK_FALSE,
        .stencilTestEnable     = VK_FALSE,
        .front                 = {},
        .back                  = {},
        .minDepthBounds        = 0.f,
        .maxDepthBounds        = 1.f
    };

    const VkPipelineColorBlendAttachmentState colorBlend {
        .blendEnable                = VK_FALSE,
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

    const VkPipelineColorBlendStateCreateInfo colorBlendState {
        .sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .pNext           = nullptr,
        .flags           = 0,
        .logicOpEnable   = VK_FALSE,
        .logicOp         = VK_LOGIC_OP_COPY,
        .attachmentCount = 1,
        .pAttachments    = &colorBlend,
        .blendConstants  = { 0.f, 0.f, 0.f, 0.f }
    };

    const VkPushConstantRange constantRange {
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        .offset     = 0,
        .size       = (uint32_t)sizeof(Common_PushConstants)
    };

    const VkPipelineLayoutCreateInfo layoutInfo {
        .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext                  = nullptr,
        .flags                  = 0,
        .setLayoutCount         = pipeline.setLayouts.count,
        .pSetLayouts            = pipeline.setLayouts.Data(),
        .pushConstantRangeCount = 1,
        .pPushConstantRanges    = &constantRange,
    };
    VkCheck(vkCreatePipelineLayout(g_contextPtr->device, &layoutInfo, nullptr, &pipeline.layout));

    const VkGraphicsPipelineCreateInfo pipelineInfo {
        .sType                      = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext                      = nullptr,
        .flags                      = 0,
        .stageCount                 = shader.stageInfos.count,
        .pStages                    = shader.stageInfos.Data(),
        .pVertexInputState          = &vertexInput,
        .pInputAssemblyState        = &inputAssembly,
        .pTessellationState         = nullptr,
        .pViewportState             = &viewportState,
        .pRasterizationState        = &rasterization,
        .pMultisampleState          = &multisampling,
        .pDepthStencilState         = &depthStencil,
        .pColorBlendState           = &colorBlendState,
        .pDynamicState              = nullptr,
        .layout                     = pipeline.layout,
        .renderPass                 = renderPass.renderPass,
        .subpass                    = 0,
        .basePipelineHandle         = VK_NULL_HANDLE,
        .basePipelineIndex          = -1
    };
    
    VkCheck(vkCreateGraphicsPipelines(g_contextPtr->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline.pipeline));
}

} //ns