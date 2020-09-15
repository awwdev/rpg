//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Helper/Initializers.hpp"

#include "GPU/Vulkan/States/GUI/GUI_RenderPass.hpp"
#include "GPU/Vulkan/States/GUI/GUI_Shader.hpp"

namespace rpg::gpu::vuk {

struct GUI_Pipeline
{
    VkPipeline pipeline;
    VkPipelineLayout layout;

    void Create(GUI_RenderPass& renderPass, GUI_Shader& shader)
    {
        const auto vertexInput      = VertexInputInfoEmpty();
        const auto inputAssembly    = InputAssemblyDefault();
        const auto viewport         = Viewport(renderPass.width, renderPass.height);
        const auto scissor          = Scissor(renderPass.width, renderPass.height);
        const auto viewportState    = ViewportState(viewport, scissor);
        const auto rasterization    = RasterizationDefault();
        const auto multisampling    = MultisamplingEmpty();
        const auto depthStencil     = DepthStencilEmpty();
        const auto blendAttachment  = BlendAttachment(VK_TRUE);
        const auto blendState       = BlendState(blendAttachment);   

        const auto layoutInfo = PipelineLayoutInfo(nullptr, 0);
        VkCheck(vkCreatePipelineLayout(g_contextPtr->device, &layoutInfo, nullptr, &layout));

        const VkGraphicsPipelineCreateInfo pipelineInfo
        {
            .sType                      = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .pNext                      = nullptr,
            .flags                      = 0,
            .stageCount                 = ArrayCount(shader.info),
            .pStages                    = shader.info,
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

    void Clear()
    {
        vkDestroyPipeline(g_contextPtr->device, pipeline, nullptr);
        vkDestroyPipelineLayout(g_contextPtr->device, layout, nullptr);
    }

    ~GUI_Pipeline()
    {
        Clear();
    }
};

}//ns