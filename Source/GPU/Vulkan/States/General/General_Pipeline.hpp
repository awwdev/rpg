//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Helper/Initializers.hpp"
#include "GPU/Vulkan/Helper/Utils.hpp"
#include "GPU/Vulkan/States/General/General_RenderPass.hpp"
#include "GPU/Vulkan/Objects/VertexBuffer.hpp"

#include "Common/Container/Array.hpp"

namespace rpg::gpu::vuk {

struct General_Pipeline
{
    VkPipeline pipeline;
    VkPipelineLayout layout;

    //Descriptor class
    
    template<class VERTEX_TYPE, auto VERTEX_COUNT>
    void Create(General_RenderPass& rp, VertexBuffer<VERTEX_TYPE, VERTEX_COUNT>& vbo)
    {
        const auto vertexInput   = vbo.VertexInputInfo();
        const auto inputAssembly = InputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

        const auto viewport      = Viewport(rp.width, rp.height);
        const auto scissor       = Scissor (rp.width, rp.height);
        const auto viewportState = ViewportState(viewport, scissor);

        const VkPipelineRasterizationStateCreateInfo rasterization 
        {
            .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .pNext                   = nullptr,
            .flags                   = 0,
            .depthClampEnable        = VK_FALSE,
            .rasterizerDiscardEnable = VK_FALSE,
            .polygonMode             = VK_POLYGON_MODE_FILL,
            .cullMode                = VK_CULL_MODE_NONE,
            .frontFace               = VK_FRONT_FACE_CLOCKWISE,
            .depthBiasEnable         = VK_FALSE,
            .depthBiasConstantFactor = 0.f,
            .depthBiasClamp          = 0.f,
            .depthBiasSlopeFactor    = 0.f,
            .lineWidth               = 1.f  
        };

         const VkPipelineMultisampleStateCreateInfo multisampling 
         {
            .sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .pNext                 = nullptr,
            .flags                 = 0,
            .rasterizationSamples  = rp.msaaSampleCount,
            .sampleShadingEnable   = VK_FALSE,
            .minSampleShading      = 0.5f,
            .pSampleMask           = nullptr,
            .alphaToCoverageEnable = VK_FALSE,
            .alphaToOneEnable      = VK_FALSE
        };

        const VkPipelineDepthStencilStateCreateInfo depthStencil 
        {
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

        const auto blendAttachment = BlendAttachent(VK_FALSE);
        const auto colorBlendState = BlendState(blendAttachment);

        const VkPushConstantRange constantRange {
            .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
            .offset     = 0,
            .size       = 0,
        };

        const auto pipelineLayoutInfo = PipelineLayoutInfo(nullptr, 0); //!
        VkCheck(vkCreatePipelineLayout(g_contextPtr->device, &pipelineLayoutInfo, nullptr, &layout));

        const VkGraphicsPipelineCreateInfo pipelineInfo 
        {
            .sType                      = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .pNext                      = nullptr,
            .flags                      = 0,
            .stageCount                 = 0, //shader.stageInfos.count, //!
            .pStages                    = 0, //shader.stageInfos.Data(), //!
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
            .renderPass                 = rp.renderPass,
            .subpass                    = 0,
            .basePipelineHandle         = VK_NULL_HANDLE,
            .basePipelineIndex          = -1
        };
        VkCheck(vkCreateGraphicsPipelines(g_contextPtr->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline));

    }

    void Clear()
    {
        vkDestroyPipelineLayout (g_contextPtr->device, layout, nullptr);
        vkDestroyPipeline       (g_contextPtr->device, pipeline, nullptr);
    }

    ~General_Pipeline()
    {
        Clear();
    }

};

}//ns