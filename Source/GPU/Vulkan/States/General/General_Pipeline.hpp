//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Helper/Initializers.hpp"

#include "GPU/Vulkan/States/General/General_RenderPass.hpp"
#include "GPU/Vulkan/States/General/General_Shader.hpp"
#include "GPU/Vulkan/States/General/General_Vertices.hpp"

namespace rpg::gpu::vuk {

struct General_Pipeline
{
    VkPipeline pipeline;
    VkPipelineLayout layout;

    void Create(General_RenderPass& renderPass, General_Shader& shader, General_Vertices& vertices)
    {
        const auto vertexInput      = VertexInputInfo(vertices.bindings, vertices.attributes);
        const auto inputAssembly    = InputAssemblyDefault();
        const auto viewport         = Viewport(renderPass.width, renderPass.height);
        const auto scissor          = Scissor(renderPass.width, renderPass.height);
        const auto viewportState    = ViewportState(viewport, scissor);
        const auto rasterization    = Rasterization();
        const auto multisampling    = Multisampling(renderPass.msaaSampleCount);
        const auto depthStencil     = DepthStencil(VK_FALSE, VK_FALSE);
        const auto blendAttachment  = BlendAttachment(VK_FALSE);
        const auto blendState       = BlendState(blendAttachment);   
        
        const auto layoutInfo = PipelineLayoutInfo(nullptr, 0);
        VkCheck(vkCreatePipelineLayout(g_contextPtr->device, &layoutInfo, nullptr, &layout));

        const VkGraphicsPipelineCreateInfo pipelineInfo
        {
            .sType                      = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .pNext                      = nullptr,
            .flags                      = 0,
            .stageCount                 = (uint32_t) ArrayCount(shader.stageInfo),
            .pStages                    = shader.stageInfo,
            .pVertexInputState          = &vertexInput,
            .pInputAssemblyState        = &inputAssembly,
            .pTessellationState         = nullptr,
            .pViewportState             = &viewportState,
            .pRasterizationState        = &rasterization,
            .pMultisampleState          = &multisampling,
            .pDepthStencilState         = &depthStencil,
            .pColorBlendState           = &blendState,
            .pDynamicState              = nullptr,
            .layout                     = layout,
            .renderPass                 = renderPass.renderPass,
            .subpass                    = 0,
            .basePipelineHandle         = VK_NULL_HANDLE,
            .basePipelineIndex          = -1
        };

        VkCheck(vkCreateGraphicsPipelines(g_contextPtr->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline));

    }

    void Destroy()
    {
        vkDestroyPipeline(g_contextPtr->device, pipeline, nullptr);
        vkDestroyPipelineLayout(g_contextPtr->device, layout, nullptr);
    }

    ~General_Pipeline()
    {
        Destroy();
    }
};

}//ns