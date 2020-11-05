//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/_Old/Pass_Shadow.hpp"
#include "gpu/Vulkan/_Old/Pass_Main.hpp"
#include "gpu/Vulkan/_Old/Pass_Post.hpp"
#include "gpu/Vulkan/_Old/Pass_GUI.hpp"

#include "res/Resources.hpp"
#include "com/ThreadPool.hpp"

namespace rpg::gpu::vuk {

struct Passes
{
    Pass_Shadow  shadow;
    Pass_Main    main;
    Pass_Post    post;
    Pass_GUI     gui;

    void Create(res::Resources& res, VkCommandPool cmdPool)
    {
        shadow  .Create(cmdPool);
        main    .Create(cmdPool, *shadow.uniforms.uboSun.activeBuffer, shadow.renderPass.shadowMaps, res);
        post    .Create(cmdPool, main.mainRenderPass.finalImage);
        gui     .Create(cmdPool, res);
    }

    void Update(gpu::RenderData& renderData, res::Resources& res)
    {
        shadow  .Update(renderData);
        main    .Update(renderData, res);
        post    .Update(renderData);
        gui     .Update(renderData);
    }

    auto Record(Commands& commands, const uint32_t cmdBufferIdx, gpu::RenderData& renderData, const res::Resources& resources)
    {
        const auto beginInfo = CommandBufferBeginInfo();
        VkCommandBuffer cmdBuffer = commands.threadCommands[0].cmdBuffers[cmdBufferIdx]; //single thread 

        VkCheck(vkBeginCommandBuffer(cmdBuffer, &beginInfo));
            
        shadow  .Record(cmdBuffer, main);
        main    .Record(cmdBuffer, renderData.main);
        post    .Record(cmdBuffer, cmdBufferIdx);
        gui     .Record(cmdBuffer, cmdBufferIdx);
            
        VkCheck(vkEndCommandBuffer(cmdBuffer));

        return com::Array<VkCommandBuffer, Commands::THREAD_COUNT_MAX> { cmdBuffer };
    }

    com::Array<VkCommandBuffer, 4> RecordMT(
    Commands& commands, const uint32_t cmdBufferIdx, 
    RenderData& renderData, com::ThreadPool<4>& threadPool)
    {
        enum PassIdx
        {
            ShadowPassIdx,
            MainPassIdx,
            PostPassIdx,
            GuiPassIdx,
        };

        const auto beginInfo = CommandBufferBeginInfo();

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

        return cmds;
    }

    void Destroy()
    {
        gui     .Destroy();
        post    .Destroy();
        shadow  .Destroy();
        main    .Destroy();
    }
};

} //ns
