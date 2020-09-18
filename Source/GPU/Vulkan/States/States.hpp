//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/States/Shadow/State_Shadow.hpp"
#include "GPU/Vulkan/States/General/State_General.hpp"
#include "GPU/Vulkan/States/Post/State_Post.hpp"
#include "GPU/Vulkan/States/GUI/State_GUI.hpp"

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
        shadow  .Create(cmdPool);
        general .Create(cmdPool);
        post    .Create(cmdPool, general);
        gui     .Create(cmdPool, hostRes);
    }

    void Update(gpu::RenderData& renderData)
    {
        shadow  .Update(renderData);
        general .Update(renderData);
        post    .Update(renderData);
        gui     .Update(renderData);
    }

    void Record(VkCommandBuffer cmdBuffer, const uint32_t cmdBufferIdx)
    {
        const auto beginInfo = CommandBufferBeginInfo();
        VkCheck(vkBeginCommandBuffer(cmdBuffer, &beginInfo));

        shadow  .Record(cmdBuffer);
        general .Record(cmdBuffer);
        post    .Record(cmdBuffer, cmdBufferIdx);
        gui     .Record(cmdBuffer, cmdBufferIdx);

        VkCheck(vkEndCommandBuffer(cmdBuffer));
    }

    void Clear()
    {
        shadow  .Clear();
        general .Clear();
        post    .Clear();
        gui     .Clear();
    }
};

} //ns