//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Wrappers/Pipeline.hpp"
#include "gpu/Vulkan/Wrappers/Shader.hpp"
#include "gpu/Vulkan/Wrappers/RenderPass.hpp"

namespace rpg::gpu::vuk {

template<auto... STAGES_T>
void CreatePipeline_GUI(Pipeline& pipeline, Shader<STAGES_T...>& shader, RenderPass& renderPass)
{
    /*
    const auto vertexInput      = VertexInputInfoEmpty();
    const auto inputAssembly    = InputAssemblyDefault();
    const auto viewport         = Viewport(renderPass.width, renderPass.height);
    const auto scissor          = Scissor(renderPass.width, renderPass.height);
    const auto viewportState    = ViewportState(viewport, scissor);
    const auto rasterization    = Rasterization();
    const auto multisampling    = Multisampling();
    const auto depthStencil     = DepthStencil();
    const auto blendAttachment  = BlendAttachment(VK_TRUE);
    const auto blendState       = BlendState(blendAttachment);   
    
    const auto layoutInfo = PipelineLayoutInfo(nullptr, 0);//!&uniforms.descriptors.descSetLayout, 1);
    VkCheck(vkCreatePipelineLayout(g_contextPtr->device, &layoutInfo, nullptr, &pipeline.layout));

    const VkGraphicsPipelineCreateInfo pipelineInfo
    {
        .sType                      = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext                      = nullptr,
        .flags                      = 0,
        .stageCount                 = shader.STAGE_COUNT,
        .pStages                    = shader.infos,
        .pVertexInputState          = &vertexInput,
        .pInputAssemblyState        = &inputAssembly,
        .pTessellationState         = nullptr,
        .pViewportState             = &viewportState,
        .pRasterizationState        = &rasterization,
        .pMultisampleState          = &multisampling,
        .pDepthStencilState         = &depthStencil,
        .pColorBlendState           = &blendState,
        .pDynamicState              = nullptr,
        .layout                     = pipeline.layout,
        .renderPass                 = renderPass.renderPass,
        .subpass                    = 0,
        .basePipelineHandle         = VK_NULL_HANDLE,
        .basePipelineIndex          = -1
    };

    VkCheck(vkCreateGraphicsPipelines(g_contextPtr->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline.pipeline));
    */
}

}//ns