//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Meta/Context.hpp"
#include "gpu/Vulkan/Helper/Initializers.hpp"

#include "gpu/Vulkan/States/Main/Main_RenderPass.hpp"
#include "gpu/Vulkan/States/Main/Main_Vertices.hpp"
#include "gpu/Vulkan/States/Main/Main_Uniforms.hpp"

#include "gpu/Vulkan/States/Main/Instances/Foliage/Foliage_Shader.hpp"

namespace rpg::gpu::vuk {

struct Foliage_Pipeline
{
    VkPipeline pipeline;
    VkPipelineLayout layout;

    void Create(
    Main_RenderPass& renderPass, 
    Foliage_Shader& shader, 
    Main_Vertices& vertices,
    Main_Uniforms& uniforms)
    {
        const auto vertexInput      = VertexInputInfo(vertices.bindings, vertices.attributes);
        const auto inputAssembly    = InputAssemblyDefault();
        const auto viewport         = Viewport(renderPass.width, renderPass.height);
        const auto scissor          = Scissor(renderPass.width, renderPass.height);
        const auto viewportState    = ViewportState(viewport, scissor);
        const auto rasterization    = Rasterization(VK_CULL_MODE_NONE, VK_POLYGON_MODE_FILL); //VK_CULL_MODE_BACK_BIT
        const auto multisampling    = Multisampling(renderPass.msaaSampleCount);
        const auto depthStencil     = DepthStencil(VK_TRUE, VK_TRUE);
        const auto blendAttachment  = BlendAttachment(VK_FALSE);
        const auto blendState       = BlendState(blendAttachment);   
        
        const auto layoutInfo = PipelineLayoutInfo(&uniforms.descriptors.descSetLayout, 1);
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

    ~Foliage_Pipeline()
    {
        Destroy();
    }
};

}//ns