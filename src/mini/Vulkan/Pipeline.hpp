#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"


namespace mini::vk
{
    struct Pipeline
    {
        VkPipeline pipeline;
        VkPipelineLayout layout;
    };


    inline Pipeline CreatePipeline_Default(Context& context)
    {
        Pipeline pipeline;

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
            .vertexBindingDescriptionCount   = 0,//!(uint32_t)bindingDescs.size(),
            .pVertexBindingDescriptions      = 0,//!bindingDescs.data(),
            .vertexAttributeDescriptionCount = 0,//!(uint32_t)attributeDescs.size(),
            .pVertexAttributeDescriptions    = 0,//!attributeDescs.data()
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
            .rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT,//!sampleCount, RENDER PASS SAMPLE COUNT MATCH
            .sampleShadingEnable   = 0,//!VK_FALSE,
            .minSampleShading      = 0,//!1.f,
            .pSampleMask           = 0,//!nullptr,
            .alphaToCoverageEnable = 0,//!alphaToCoverage,
            .alphaToOneEnable      = 0,//!alphaToOneEnable,
        };

        const VkPipelineLayoutCreateInfo layoutInfo {
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext                  = nullptr,
            .flags                  = 0,
            .setLayoutCount         = 0,//!(uint32_t)descSetLayouts.size(), 
            .pSetLayouts            = 0,//!descSetLayouts.data(),
            .pushConstantRangeCount = 0,//!pushConstantRangeCount,
            .pPushConstantRanges    = 0,//!pushConstantRanges
        };
        VK_CHECK(vkCreatePipelineLayout(context.device, &layoutInfo, nullptr, &pipeline.layout));


        const VkGraphicsPipelineCreateInfo pipelineInfo {
            .sType                      = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .pNext                      = nullptr,
            .flags                      = 0,
            .stageCount                 = 0,//!(uint32_t)stages.size(),
            .pStages                    = 0,//!stages.data(),
            .pVertexInputState          = 0,//!&vertexInput,
            .pInputAssemblyState        = 0,//!&assembly,
            .pTessellationState         = 0,//!nullptr,
            .pViewportState             = 0,//!&viewportState,
            .pRasterizationState        = 0,//!&rasterization,
            .pMultisampleState          = 0,//!&multisampling,
            .pDepthStencilState         = 0,//!&depthStencil,
            .pColorBlendState           = 0,//!&colorBlendState,
            .pDynamicState              = 0,//!nullptr,
            .layout                     = 0,//!pipeline.layout,
            .renderPass                 = 0,//!renderPassRef->renderPass,
            .subpass                    = 0,
            .basePipelineHandle         = VK_NULL_HANDLE,
            .basePipelineIndex          = -1
        };
        VK_CHECK(vkCreateGraphicsPipelines(context.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline.pipeline));


        return pipeline;
    }

}//ns