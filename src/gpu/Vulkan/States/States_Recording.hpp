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
    auto* rp_shadow_info = &resources.renderPasses.shadowRenderPass.beginInfos[cmdBufferIdx];
    auto* rp_main_info   = &resources.renderPasses.mainRenderPass.beginInfos[cmdBufferIdx];
    auto* rp_post_info   = &resources.renderPasses.postRenderPass.beginInfos[cmdBufferIdx];
    
    //? shadow pass
    vkCmdBeginRenderPass        (cmdBuffer, rp_shadow_info, VK_SUBPASS_CONTENTS_INLINE);
        Record_TerrainShadow    (cmdBuffer, cmdBufferIdx, resources);
        Record_InstancesShadow  (cmdBuffer, cmdBufferIdx, resources);
    vkCmdEndRenderPass          (cmdBuffer);

    //? main pass
    vkCmdBeginRenderPass        (cmdBuffer, rp_main_info, VK_SUBPASS_CONTENTS_INLINE);
        Record_Terrain          (cmdBuffer, cmdBufferIdx, resources);
        Record_Instances        (cmdBuffer, cmdBufferIdx, resources);
    vkCmdEndRenderPass          (cmdBuffer);

    //? post pass
    vkCmdBeginRenderPass        (cmdBuffer, rp_post_info, VK_SUBPASS_CONTENTS_INLINE);
        Record_Post             (cmdBuffer, cmdBufferIdx, resources);
        Record_GUI              (cmdBuffer, cmdBufferIdx, resources);
    vkCmdEndRenderPass          (cmdBuffer);
}

}//ns