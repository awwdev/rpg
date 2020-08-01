//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Objects/Buffer.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Utils/Structs.hpp"

namespace mini::vk
{
    struct VertexGroup
    {
        u32 begin, count;
    };

    //bakable
    template<class T, u32 N>
    struct VertexBuffer
    {
        using VERTEX_TYPE = T;
        static constexpr u32 MAX_VERTEX_COUNT = N;
        static constexpr u32 TOTAL_SIZE = N * sizeof(T);

        Buffer  cpuBuffer; //host visible
        Buffer  gpuBuffer; //device local
        Buffer* activeBuffer = nullptr;

        box::Array<VertexGroup, 100> vertexGroups;
        u32 count = 0;
        u32 CurrentSize() const { return sizeof(T) * count; }

        //? need to be filled by "factory" method
        box::Array<VkVertexInputBindingDescription, 2>   bindings;
        box::Array<VkVertexInputAttributeDescription, 4> attributes;
       
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
            gpuBuffer.Create(
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                TOTAL_SIZE,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            );
            activeBuffer = &gpuBuffer;

            const VkCommandBufferAllocateInfo allocInfo {
                .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                .pNext              = nullptr,
                .commandPool        = cmdPool,
                .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                .commandBufferCount = 1
            };
            VkCommandBuffer commandBuffer;
            VK_CHECK(vkAllocateCommandBuffers(g_contextPtr->device, &allocInfo, &commandBuffer));

            const VkCommandBufferBeginInfo beginInfo{
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
            };
            VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo));

            const VkBufferCopy copyRegion {
                .srcOffset = 0,
                .dstOffset = 0,
                .size = CurrentSize()
            };
            vkCmdCopyBuffer(commandBuffer, cpuBuffer.buffer, gpuBuffer.buffer, 1, &copyRegion);

            VK_CHECK(vkEndCommandBuffer(commandBuffer));

            const VkSubmitInfo submitInfo {
                .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                .commandBufferCount = 1,
                .pCommandBuffers = &commandBuffer
            };
            VK_CHECK(vkQueueSubmit(g_contextPtr->queue, 1, &submitInfo, VK_NULL_HANDLE));
            VK_CHECK(vkQueueWaitIdle(g_contextPtr->queue));

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

        void Clear()
        {
            count = 0;
            vertexGroups.Clear();
        }        
    };

    template<class T, u32 N>
    auto CreatePipelineVertexInputInfo(VertexBuffer<T, N>& vb) -> VkPipelineVertexInputStateCreateInfo
    {
        return {
            .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .pNext                           = nullptr,
            .flags                           = 0,
            .vertexBindingDescriptionCount   = vb.bindings.count,
            .pVertexBindingDescriptions      = vb.bindings.Data(),
            .vertexAttributeDescriptionCount = vb.attributes.count,
            .pVertexAttributeDescriptions    = vb.attributes.Data()
        };
    }

    auto CreatePipelineInputAssemblyInfo(const VkPrimitiveTopology topology)
    {
        return VkPipelineInputAssemblyStateCreateInfo {
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .pNext                  = nullptr,
            .flags                  = 0,
            .topology               = topology,
            .primitiveRestartEnable = VK_FALSE 
        };
    }

}//ns