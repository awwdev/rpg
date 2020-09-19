//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Helper/Initializers.hpp"

namespace rpg::gpu::vuk {

struct General_Cmd
{
    vuk::VkArray<VkCommandBuffer, 4> cmdBuffers;

    void Create(VkCommandPool cmdPool)
    {
        cmdBuffers.count = g_contextPtr->swapImages.count;
        const auto allocInfo = CmdBufferAllocInfo(cmdPool, cmdBuffers.count);
        VkCheck(vkAllocateCommandBuffers(g_contextPtr->device, &allocInfo, cmdBuffers.data));
    }

    void Destroy()
    {
        vkFreeCommandBuffers(g_contextPtr->device, cmdPool, cmdBuffers.count, cmdBuffers.data);
    }
    ~General_Cmd() { Destroy(); }

};

}//ns