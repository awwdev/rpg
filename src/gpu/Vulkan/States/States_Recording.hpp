//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/States/Recording/Recording_GUI.hpp"
#include "gpu/Vulkan/States/Recording/Recording_Instances.hpp"
#include "gpu/Vulkan/States/Recording/Recording_Terrain.hpp"
#include "gpu/Vulkan/States/Recording/Recording_Post.hpp"

#include "gpu/Vulkan/States/States_Resources.hpp"
#include "gpu/RenderData/RenderData_Shadow.hpp"
#include "gpu/Vulkan/Commands.hpp"

namespace rpg::gpu::vuk {

inline auto Record(Commands& commands, const uint32_t cmdBufferIdx, Resources& resources)
{
    VkCommandBuffer cmdBuffer = commands.threadCommands[0].cmdBuffers[cmdBufferIdx]; //single thread 
    const auto beginInfo = CommandBufferBeginInfo();
    VkCheck(vkBeginCommandBuffer(cmdBuffer, &beginInfo));

    //? shadow pass
    for(uint32_t cascadeIdx = 0; cascadeIdx < RenderData_Shadow::CASCADE_COUNT; ++cascadeIdx)
    {
        auto* rp_shadow_info = &resources.renderPasses.shadowRenderPass.beginInfos[cascadeIdx];
        vkCmdBeginRenderPass(cmdBuffer, rp_shadow_info, VK_SUBPASS_CONTENTS_INLINE);
        Record_TerrainShadow(cmdBuffer, cmdBufferIdx, resources, cascadeIdx);
        Record_InstancesShadow(cmdBuffer, cmdBufferIdx, resources, cascadeIdx);
        vkCmdEndRenderPass(cmdBuffer);
    }

    //? main pass
    auto* rp_main_info = &resources.renderPasses.mainRenderPass.beginInfos[0];
    vkCmdBeginRenderPass(cmdBuffer, rp_main_info, VK_SUBPASS_CONTENTS_INLINE);
    Record_Terrain(cmdBuffer, cmdBufferIdx, resources);
    Record_Instances(cmdBuffer, cmdBufferIdx, resources);
    vkCmdEndRenderPass(cmdBuffer);

    //? post pass
    auto* rp_post_info = &resources.renderPasses.postRenderPass.beginInfos[cmdBufferIdx];
    vkCmdBeginRenderPass(cmdBuffer, rp_post_info, VK_SUBPASS_CONTENTS_INLINE);
    Record_Post(cmdBuffer, cmdBufferIdx, resources);
    Record_GUI(cmdBuffer, cmdBufferIdx, resources);
    vkCmdEndRenderPass(cmdBuffer);

    VkCheck(vkEndCommandBuffer(cmdBuffer));

    return com::Array<VkCommandBuffer, Commands::THREAD_COUNT_MAX> { cmdBuffer };
}

}//ns

/*
//! multi threaded command buffer recording

com::Array<VkCommandBuffer, 4> cmds;
VkCommandBuffer cmdBuffer = nullptr;

cmdBuffer = cmds.AppendElement(commands.threadCommands[ShadowPassIdx].cmdBuffers[cmdBufferIdx]);
threadPool.AssignTask(ShadowPassIdx, [this, cmdBuffer, beginInfo, cmdBufferIdx]{
    VkCheck(vkBeginCommandBuffer(cmdBuffer, &beginInfo));
    shadow.Record(cmdBuffer, main);
    VkCheck(vkEndCommandBuffer(cmdBuffer));
});

cmdBuffer = cmds.AppendElement(commands.threadCommands[MainPassIdx].cmdBuffers[cmdBufferIdx]);
threadPool.AssignTask(MainPassIdx, [this, cmdBuffer, beginInfo, cmdBufferIdx, &renderData]{
    VkCheck(vkBeginCommandBuffer(cmdBuffer, &beginInfo));
    main.Record(cmdBuffer, renderData.main);
    VkCheck(vkEndCommandBuffer(cmdBuffer));
});
    
cmdBuffer = cmds.AppendElement(commands.threadCommands[PostPassIdx].cmdBuffers[cmdBufferIdx]);
threadPool.AssignTask(PostPassIdx, [this, cmdBuffer, beginInfo, cmdBufferIdx]{
    VkCheck(vkBeginCommandBuffer(cmdBuffer, &beginInfo));
    post.Record(cmdBuffer, cmdBufferIdx);
    VkCheck(vkEndCommandBuffer(cmdBuffer));
});

cmdBuffer = cmds.AppendElement(commands.threadCommands[GuiPassIdx].cmdBuffers[cmdBufferIdx]);
threadPool.AssignTask(GuiPassIdx, [this, cmdBuffer, beginInfo, cmdBufferIdx]{
    VkCheck(vkBeginCommandBuffer(cmdBuffer, &beginInfo));
    gui.Record(cmdBuffer, cmdBufferIdx);
    VkCheck(vkEndCommandBuffer(cmdBuffer));
});

threadPool.WaitForAllTasks();
*/