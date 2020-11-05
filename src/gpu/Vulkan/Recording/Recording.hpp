//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Recording/Recording_GUI.hpp"
#include "gpu/Vulkan/Recording/Recording_Instances.hpp"
#include "gpu/Vulkan/Recording/Recording_Terrain.hpp"
#include "gpu/Vulkan/Recording/Recording_Post.hpp"

#include "gpu/Vulkan/Resources/Resources.hpp"
#include "gpu/Vulkan/Abstraction/Meta/Commands.hpp"

namespace rpg::gpu::vuk {

inline void Record(Commands& commands, const uint32_t cmdBufferIdx, Resources& resources)
{
    VkCommandBuffer cmdBuffer = commands.threadCommands[0].cmdBuffers[cmdBufferIdx]; //single thread 

    Record_TerrainShadow    (cmdBuffer, cmdBufferIdx, resources);
    Record_InstancesShadow  (cmdBuffer, cmdBufferIdx, resources);
    Record_Terrain          (cmdBuffer, cmdBufferIdx, resources);
    Record_Instances        (cmdBuffer, cmdBufferIdx, resources);
    Record_Post             (cmdBuffer, cmdBufferIdx, resources);
    Record_GUI              (cmdBuffer, cmdBufferIdx, resources);
}

}//ns