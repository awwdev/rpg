//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/States/Shadow/State_Shadow.hpp"
#include "gpu/Vulkan/States/General/State_General.hpp"
#include "gpu/Vulkan/States/Post/State_Post.hpp"
#include "gpu/Vulkan/States/gui/State_GUI.hpp"

#include "res/Resources.hpp"
#include "com/ThreadPool.hpp"

namespace rpg::gpu::vuk {

struct States
{
    State_Shadow  shadow;
    State_General general;
    State_Post    post;
    State_GUI     gui;

    void Create(res::Resources& res, VkCommandPool cmdPool)
    {
        shadow  .Create(cmdPool);
        general .Create(cmdPool, *shadow.uniforms.uboSun.activeBuffer, shadow.renderPass.shadowMaps, res);
        post    .Create(cmdPool, general.generalRenderPass.finalImage);
        gui     .Create(cmdPool, res);
    }

    void Update(gpu::RenderData& renderData, res::Resources& res)
    {
        shadow  .Update(renderData);
        general .Update(renderData, res);
        post    .Update(renderData);
        gui     .Update(renderData);
    }

    auto Record(Commands& commands, const uint32_t cmdBufferIdx, gpu::RenderData& renderData, const res::Resources& resources)
    {
        const auto beginInfo = CommandBufferBeginInfo();
        VkCommandBuffer cmdBuffer = commands.threadCommands[0].cmdBuffers[cmdBufferIdx]; //single thread 

        VkCheck(vkBeginCommandBuffer(cmdBuffer, &beginInfo));
            
        shadow  .Record(cmdBuffer, general);
        general .Record(cmdBuffer, renderData.general);
        post    .Record(cmdBuffer, cmdBufferIdx);
        gui     .Record(cmdBuffer, cmdBufferIdx);
            
        VkCheck(vkEndCommandBuffer(cmdBuffer));

        return com::Array<VkCommandBuffer, Commands::THREAD_COUNT_MAX> { cmdBuffer };
    }

    com::Array<VkCommandBuffer, 4> RecordMT(
    Commands& commands, const uint32_t cmdBufferIdx, 
    RenderData& renderData, com::ThreadPool<4>& threadPool)
    {
        constexpr auto SHADOW_STATE_IDX  = 0;
        constexpr auto GENERAL_STATE_IDX = 1;
        constexpr auto POST_STATE_IDX    = 2;
        constexpr auto GUI_STATE_IDX     = 3;

        const auto beginInfo = CommandBufferBeginInfo();

        com::Array<VkCommandBuffer, 4> cmds;
        VkCommandBuffer cmdBuffer = nullptr;

        cmdBuffer = cmds.Append(commands.threadCommands[SHADOW_STATE_IDX].cmdBuffers[cmdBufferIdx]);
        threadPool.AssignTask(0, [this, cmdBuffer, beginInfo, cmdBufferIdx]{
            VkCheck(vkBeginCommandBuffer(cmdBuffer, &beginInfo));
            shadow.Record(cmdBuffer, general);
            VkCheck(vkEndCommandBuffer(cmdBuffer));
        });
        
        cmdBuffer = cmds.Append(commands.threadCommands[GENERAL_STATE_IDX].cmdBuffers[cmdBufferIdx]);
        threadPool.AssignTask(1, [this, cmdBuffer, beginInfo, cmdBufferIdx, &renderData]{
            VkCheck(vkBeginCommandBuffer(cmdBuffer, &beginInfo));
            general.Record(cmdBuffer, renderData.general);
            VkCheck(vkEndCommandBuffer(cmdBuffer));
        });
            
        cmdBuffer = cmds.Append(commands.threadCommands[POST_STATE_IDX].cmdBuffers[cmdBufferIdx]);
        threadPool.AssignTask(2, [this, cmdBuffer, beginInfo, cmdBufferIdx]{
            VkCheck(vkBeginCommandBuffer(cmdBuffer, &beginInfo));
            post.Record(cmdBuffer, cmdBufferIdx);
            VkCheck(vkEndCommandBuffer(cmdBuffer));
        });

        cmdBuffer = cmds.Append(commands.threadCommands[GUI_STATE_IDX].cmdBuffers[cmdBufferIdx]);
        threadPool.AssignTask(3, [this, cmdBuffer, beginInfo, cmdBufferIdx]{
            VkCheck(vkBeginCommandBuffer(cmdBuffer, &beginInfo));
            gui.Record(cmdBuffer, cmdBufferIdx);
            VkCheck(vkEndCommandBuffer(cmdBuffer));
        });

        threadPool.WaitForAllTasks();

        return cmds;
    }

    void Destroy()
    {
        gui     .Destroy();
        post    .Destroy();
        shadow  .Destroy();
        general .Destroy();
    }
};

} //ns
