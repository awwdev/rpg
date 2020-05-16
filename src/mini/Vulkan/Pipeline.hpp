#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Ctors.hpp"
#include "mini/Vulkan/Shader.hpp"
#include "mini/Vulkan/RenderPass.hpp"

namespace mini::vk
{

    //? DEDICATED STRUCTS

    struct Default_Pipeline
    {
        VkDevice device;

        VkPipeline pipeline;
        VkPipelineLayout layout;
        

        explicit Default_Pipeline(
            Context& context, 
            Default_Shader& shader, 
            Default_RenderPass& renderPass)
            : device { context.device }
        {
            const VkPipelineInputAssemblyStateCreateInfo inputAssembly {
                .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
                .pNext                  = nullptr,
                .flags                  = 0,
                .topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                .primitiveRestartEnable = VK_FALSE 
            };

            const VkPipelineVertexInputStateCreateInfo vertexInput {
                .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
                .pNext                           = nullptr,
                .flags                           = 0,
                .vertexBindingDescriptionCount   = ARRAY_COUNT(shader.BINDING_DESCS),
                .pVertexBindingDescriptions      = shader.BINDING_DESCS,
                .vertexAttributeDescriptionCount = ARRAY_COUNT(shader.ATTRIBUTE_DESCS),
                .pVertexAttributeDescriptions    = shader.ATTRIBUTE_DESCS,
            };

            const VkViewport viewport {
                .x        = 0.f,
                .y        = 0.f,
                .width    = (float)context.surfaceCapabilities.currentExtent.width,
                .height   = (float)context.surfaceCapabilities.currentExtent.height,
                .minDepth = 0.f,
                .maxDepth = 1.f
            };

            const VkRect2D scissor { 
                .offset = VkOffset2D { 0, 0 },
                .extent = VkExtent2D 
                { 
                    context.surfaceCapabilities.currentExtent.width, 
                    context.surfaceCapabilities.currentExtent.height 
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
                .cullMode                = VK_CULL_MODE_BACK_BIT,
                .frontFace               = VK_FRONT_FACE_CLOCKWISE,
                .depthBiasEnable         = VK_FALSE,
                .depthBiasConstantFactor = 0.f,
                .depthBiasClamp          = 0.f,
                .depthBiasSlopeFactor    = 0.f,
                .lineWidth               = 1.f  
            };

            const VkPipelineMultisampleStateCreateInfo multisampling {
                .sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
                .pNext                 = nullptr,
                .flags                 = 0,
                .rasterizationSamples  = renderPass.SAMPLE_COUNT,
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
                .depthTestEnable       = VK_FALSE,
                .depthWriteEnable      = VK_FALSE,
                .depthCompareOp        = VK_COMPARE_OP_LESS,
                .depthBoundsTestEnable = VK_FALSE,
                .stencilTestEnable     = VK_FALSE,
                .front                 = {},
                .back                  = {},
                .minDepthBounds        = 0.f,
                .maxDepthBounds        = 1.f
            };

            const VkPipelineColorBlendAttachmentState colorBlend {
                .blendEnable                = VK_TRUE,
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

            const VkPipelineLayoutCreateInfo layoutInfo {
                .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
                .pNext                  = nullptr,
                .flags                  = 0,
                .setLayoutCount         = 0,        //!add uniform data
                .pSetLayouts            = nullptr,
                .pushConstantRangeCount = 0,
                .pPushConstantRanges    = nullptr,
            };
            VK_CHECK(vkCreatePipelineLayout(context.device, &layoutInfo, nullptr, &layout));


            const VkGraphicsPipelineCreateInfo pipelineInfo {
                .sType                      = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
                .pNext                      = nullptr,
                .flags                      = 0,
                .stageCount                 = ARRAY_COUNT(shader.stages),
                .pStages                    = shader.stages,
                .pVertexInputState          = &vertexInput,
                .pInputAssemblyState        = &inputAssembly,
                .pTessellationState         = nullptr,
                .pViewportState             = &viewportState,
                .pRasterizationState        = &rasterization,
                .pMultisampleState          = &multisampling,
                .pDepthStencilState         = &depthStencil,
                .pColorBlendState           = &colorBlendState,
                .pDynamicState              = nullptr,
                .layout                     = layout,
                .renderPass                 = renderPass.renderPass,
                .subpass                    = 0,
                .basePipelineHandle         = VK_NULL_HANDLE,
                .basePipelineIndex          = -1
            };
            
            VK_CHECK(vkCreateGraphicsPipelines(context.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline));
        }

        ~Default_Pipeline()
        {
            vkDestroyPipelineLayout(device, layout, nullptr);
            vkDestroyPipeline(device, pipeline, nullptr);
        }

    };

} //ns