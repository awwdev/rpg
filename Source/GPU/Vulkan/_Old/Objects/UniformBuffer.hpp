//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/_Old/Objects/Buffer.hpp"
#include "GPU/RenderData/_Old/RenderStructs.hpp"

namespace rpg::gpu::vuk {

struct UniformInfo
{
    enum Type { Buffer, Image } type;
    VkDescriptorSetLayoutBinding binding {}; //includes binding
    //union {
        VkDescriptorBufferInfo bufferInfo {};
        VkDescriptorImageInfo  imageInfo  {};
    //};
};

//groups are only referenced from rendergraph!
template<class T, u32 MAX_COUNT_T>
struct UniformBuffer_Groups
{
    static constexpr u32 MAX_COUNT  = MAX_COUNT_T;
    static constexpr u32 TOTAL_SIZE = sizeof(T) * MAX_COUNT_T;
    using  TYPE = T;

    BufferOld      buffer;
    UniformInfo info { .type = UniformInfo::Buffer }; //!complete in factory

    u32 count = 0;
    u32 CurrentSize() const { return sizeof(T) * count; }

    void Create(VkMemoryPropertyFlags memFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
    {
        buffer.Create(
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            TOTAL_SIZE,
            memFlags
        );
        buffer.Map();
    }

    template<u32 GROUPS_COUNT>
    void Store(const gpu::UniformDataGroups<T, MAX_COUNT_T, GROUPS_COUNT>& hostUBO)
    {
        buffer.Store(hostUBO.data.bytes, hostUBO.data.CurrentSize()); //no offset, all at once
        count = hostUBO.data.count;
    }

    template<u32 GROUPS_COUNT>
    void Store(const gpu::UniformDataGroupsMesh<T, MAX_COUNT_T, GROUPS_COUNT>& hostUBO)
    {
        buffer.Store(hostUBO.data.bytes, hostUBO.data.CurrentSize()); //no offset, all at once
        count = hostUBO.data.count;
    }

    void Reset()
    {
        count = 0;
    }

    void Clear()
    {
        buffer.Clear();
    }

    ~UniformBuffer_Groups()
    {
        Clear();
    }

};

template<class T, u32 MAX_COUNT_T>
struct UniformBufferOld
{
    static constexpr u32 MAX_COUNT  = MAX_COUNT_T;
    static constexpr u32 TOTAL_SIZE = sizeof(T) * MAX_COUNT_T;
    using  TYPE = T;

    BufferOld  cpuBuffer;
    BufferOld  gpuBuffer;
    BufferOld* activeBuffer = nullptr;
    bool IsBaked() const { return activeBuffer == &gpuBuffer; }

    u32 count = 0;
    u32 CurrentSize() const { return sizeof(T) * count; }

    void Create(VkMemoryPropertyFlags memFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
    {
        cpuBuffer.Create(
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            TOTAL_SIZE,
            memFlags
        );
        cpuBuffer.Map();
    }

    void Store(const T& data)
    {
        cpuBuffer.Store(&data, sizeof(T), 0);
        ++count;
    }

    void Reset()
    {
        count = 0;
    }

    ~UniformBufferOld()
    {
        Clear();
    }

    void Clear()
    {
        cpuBuffer.Clear();
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

};

}//ns