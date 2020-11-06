//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/States/Recording/Recording_GUI.hpp"
#include "gpu/Vulkan/States/Recording/Recording_Instances.hpp"
#include "gpu/Vulkan/States/Recording/Recording_Terrain.hpp"
#include "gpu/Vulkan/States/Recording/Recording_Post.hpp"

#include "gpu/Vulkan/States/States_Resources.hpp"
#include "gpu/Vulkan/Abstraction/Meta/Commands.hpp"

namespace rpg::gpu::vuk {

inline void Record(Commands& commands, const uint32_t cmdBufferIdx, Resources& resources)
{
    VkCommandBuffer cmdBuffer = commands.threadCommands[0].cmdBuffers[cmdBufferIdx]; //single thread 
    auto& rp_shadow = resources.renderPasses.shadowRenderPass;
    auto& rp_main = resources.renderPasses.mainRenderPass;
    auto& rp_post = resources.renderPasses.postRenderPass;
    
    //? shadow pass
    vkCmdBeginRenderPass        (cmdBuffer, &rp_shadow.beginInfos[cmdBufferIdx], VK_SUBPASS_CONTENTS_INLINE);
        Record_TerrainShadow    (cmdBuffer, cmdBufferIdx, resources);
        Record_InstancesShadow  (cmdBuffer, cmdBufferIdx, resources);
    vkCmdEndRenderPass          (cmdBuffer);

    //? main pass
    vkCmdBeginRenderPass        (cmdBuffer, &rp_main.beginInfos[cmdBufferIdx], VK_SUBPASS_CONTENTS_INLINE);
        Record_Terrain          (cmdBuffer, cmdBufferIdx, resources);
        Record_Instances        (cmdBuffer, cmdBufferIdx, resources);
    vkCmdEndRenderPass          (cmdBuffer);

    //? post pass
    vkCmdBeginRenderPass        (cmdBuffer, &rp_post.beginInfos[cmdBufferIdx], VK_SUBPASS_CONTENTS_INLINE);
        Record_Post             (cmdBuffer, cmdBufferIdx, resources);
        Record_GUI              (cmdBuffer, cmdBufferIdx, resources);
    vkCmdEndRenderPass          (cmdBuffer);
}

}//ns