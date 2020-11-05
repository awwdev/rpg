//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Abstraction/Meta/Context.hpp"
#include "gpu/Vulkan/Abstraction/Objects/Pipeline.hpp"
#include "gpu/Vulkan/Abstraction/Objects/RenderPass.hpp"

namespace rpg::gpu::vuk {

inline void Record_GUI(VkCommandBuffer cmdBuffer, uint32_t const cmdBufferIdx, RenderPass& renderPass, Pipeline& pipeline)
{
    //if (uniforms.uboText.Count() == 0){ //!
        vkCmdBeginRenderPass(cmdBuffer, &renderPass.beginInfos[cmdBufferIdx], VK_SUBPASS_CONTENTS_INLINE);
        vkCmdEndRenderPass(cmdBuffer);
        return;
    //}

    vkCmdBeginRenderPass    (cmdBuffer, &renderPass.beginInfos[cmdBufferIdx], VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);
    //vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.layout, 0, 
    //                         uniforms.descriptors.descSets.count, uniforms.descriptors.descSets.data, 0, nullptr); //!
    //vkCmdDraw               (cmdBuffer, uniforms.uboText.count * 6, 1, 0, 0); //!
    vkCmdEndRenderPass      (cmdBuffer);
}

}//ns