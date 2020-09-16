//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/States/State_Shadow.hpp"
#include "GPU/Vulkan/States/State_General.hpp"
#include "GPU/Vulkan/States/State_Post.hpp"
#include "GPU/Vulkan/States/State_GUI.hpp"

#include "Resources/CpuResources.hpp"

namespace rpg::gpu::vuk {

struct States
{
    State_Shadow  shadow;
    State_General general;
    State_Post    post;
    State_GUI     gui;

    void Create(res::HostResources& hostRes, VkCommandPool cmdPool)
    {
        shadow.Create(cmdPool);
        general.Create(cmdPool);
        post.Create(cmdPool);
        gui.Create(hostRes, cmdPool);
    }

    void Update(gpu::RenderData& renderData)
    {
        shadow.Update(renderData);
        general.Update(renderData);
        post.Update(renderData);
        gui.Update(renderData);
    }

    void Record(VkCommandBuffer cmdBuffer, const uint32_t cmdBufferIdx)
    {
        const auto beginInfo = CommandBufferBeginInfo();
        VkCheck(vkBeginCommandBuffer(cmdBuffer, &beginInfo));

        shadow.Record(cmdBuffer);
        general.Record(cmdBuffer);
        post.Record(cmdBuffer);
        gui.Record(cmdBuffer, cmdBufferIdx);

        VkCheck(vkEndCommandBuffer(cmdBuffer));
    }

    void Clear()
    {
        shadow.Clear();
        general.Clear();
        post.Clear();
        gui.Clear();
    }
};

} //ns