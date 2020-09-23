//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Meta/Context.hpp"
#include "gpu/Vulkan/_Old/Objects/Buffer.hpp"
#include "com/Container/Array.hpp"
#include "com/Structs.hpp"

namespace rpg::gpu::vuk {

struct VertexGroup
{
    u32 begin, count;
};

//bakable
template<class T, u32 N>
struct VertexBufferOld
{
    using VERTEX_TYPE = T;
    static constexpr u32 MAX_VERTEX_COUNT = N;
    static constexpr u32 TOTAL_SIZE = N * sizeof(T);

    BufferOld  cpuBuffer; //host visible
    BufferOld  gpuBuffer; //device local
    BufferOld* activeBuffer = nullptr;

    bool IsBaked() const { return activeBuffer == &gpuBuffer; }

    com::Array<VertexGroup, 100> vertexGroups;
    u32 count = 0;
    u32 CurrentSize() const { return sizeof(T) * count; }

    //? need to be filled by "factory" method
    com::Array<VkVertexInputBindingDescription, 2>   bindings;
    com::Array<VkVertexInputAttributeDescription, 4> attributes;
    
    void Create()
    {
        cpuBuffer.Create(
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            TOTAL_SIZE,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );
        cpuBuffer.Map();
        activeBuffer = &cpuBuffer;
    }

    void Bake(VkCommandPool cmdPool)
    {
        if (activeBuffer == &gpuBuffer)
            gpuBuffer.Clear();
        activeBuffer = &gpuBuffer;

        gpuBuffer.Create(
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            TOTAL_SIZE,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        const VkCommandBufferAllocateInfo allocInfo {
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext              = nullptr,
            .commandPool        = cmdPool,
            .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1
        };
        VkCommandBuffer commandBuffer;
        VkCheck(vkAllocateCommandBuffers(g_contextPtr->device, &allocInfo, &commandBuffer));

        const VkCommandBufferBeginInfo beginInfo{
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext              = 0,
            .flags              = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
            .pInheritanceInfo   = 0,
        };
        VkCheck(vkBeginCommandBuffer(commandBuffer, &beginInfo));

        const VkBufferCopy copyRegion {
            .srcOffset = 0,
            .dstOffset = 0,
            .size = CurrentSize()
        };
        vkCmdCopyBuffer(commandBuffer, cpuBuffer.buffer, gpuBuffer.buffer, 1, &copyRegion);

        VkCheck(vkEndCommandBuffer(commandBuffer));

        const VkSubmitInfo submitInfo {
            .sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext                  = 0,
            .waitSemaphoreCount     = 0,
            .pWaitSemaphores        = 0,
            .pWaitDstStageMask      = 0,
            .commandBufferCount     = 1,
            .pCommandBuffers        = &commandBuffer,
            .signalSemaphoreCount   = 0,
            .pSignalSemaphores      = 0
        };
        VkCheck(vkQueueSubmit(g_contextPtr->queue, 1, &submitInfo, VK_NULL_HANDLE));
        VkCheck(vkQueueWaitIdle(g_contextPtr->queue));

        vkFreeCommandBuffers(g_contextPtr->device, cmdPool, 1, &commandBuffer);
    }

    void StoreGroup(const T* const vertices, const u32 pCount)
    {
        cpuBuffer.Store(vertices, pCount * sizeof(T), CurrentSize());
        vertexGroups.Append(count, pCount);
        count += pCount;
    }

    template<u32 COUNT>
    void StoreGroup(const T(&arr)[COUNT])
    {
        cpuBuffer.Store(arr, COUNT * sizeof(T), CurrentSize());
        vertexGroups.Append(count, COUNT);
        count += COUNT;
    }

    template<u32 COUNT>
    void UpdateGroup(const u32 groupIdx, const T(&arr)[COUNT])
    {
        const auto& group = vertexGroups[groupIdx];
        cpuBuffer.Store(arr, COUNT * sizeof(T), group.begin * sizeof(T));
    }

    void Reset()
    {
        count = 0;
        vertexGroups.Clear();
    }     

    void Clear()
    {
        gpuBuffer.Clear();
        cpuBuffer.Clear();
        Reset();
    }

    ~VertexBufferOld()
    {
        Clear();
    }

    auto VertexInputInfo() 
    {
        return VkPipelineVertexInputStateCreateInfo {
            .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .pNext                           = nullptr,
            .flags                           = 0,
            .vertexBindingDescriptionCount   = bindings.count,
            .pVertexBindingDescriptions      = bindings.Data(),
            .vertexAttributeDescriptionCount = attributes.count,
            .pVertexAttributeDescriptions    = attributes.Data()
        };
    }
};

}//ns