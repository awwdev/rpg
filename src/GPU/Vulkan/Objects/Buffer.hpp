//https://github.com/awwdev

#pragma once

#include "GPU/Vulkan/Meta/Context.hpp"
#include "Common/Structs.hpp"
#include "Debug/Assert.hpp"

namespace rpg::gpu::vuk {

inline VkMemoryAllocateInfo CreateAllocInfo(const VkDeviceSize& size, const uint32_t memTypeIndex)
{
    return {
        .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext           = nullptr,
        .allocationSize  = size,
        .memoryTypeIndex = memTypeIndex
    };
}

inline uint32_t GetMemoryType(
    const VkPhysicalDeviceMemoryProperties& physicalMemProps,
    const VkMemoryRequirements& memReqs,
    const VkMemoryPropertyFlags neededMemProps) 
{
    for (uint32_t i = 0; i < physicalMemProps.memoryTypeCount; ++i) {
        if (memReqs.memoryTypeBits & (1 << i) &&
            (physicalMemProps.memoryTypes[i].propertyFlags & neededMemProps) == neededMemProps) 
            return i;
    }
    dbg::LogError("no suitable memory type found!");
    return {};
}


struct Buffer
{
    VkBuffer        buffer = nullptr;
    VkDeviceMemory  memory = nullptr;
    void*           memPtr = nullptr;
    std::size_t     size   = 0;

    void Map()   { VkCheck(vkMapMemory(g_contextPtr->device, memory, 0, size, 0, &memPtr)); }
    void Unmap() { vkUnmapMemory(g_contextPtr->device, memory); }

    void Store(const void* const data, const size_t size, const size_t offset = 0)
    {
        dbg::Assert(data != nullptr, "data is nullptr"); //UB on memcpy
        std::memcpy((char*)memPtr + offset, data, size);
    }

    void Create(
        const VkBufferUsageFlags usage, 
        const std::size_t pSize, 
        const VkMemoryPropertyFlags memProps)
    {
        size = pSize; 

        //? BUFFER
        const VkBufferCreateInfo bufferInfo {
            .sType                  = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext                  = nullptr,
            .flags                  = 0,
            .size                   = size,
            .usage                  = usage,
            .sharingMode            = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount  = 0,
            .pQueueFamilyIndices    = nullptr
        };
        VkCheck(vkCreateBuffer(g_contextPtr->device, &bufferInfo, nullptr, &buffer));

        //? MEMORY
        VkMemoryRequirements memReqs;
        vkGetBufferMemoryRequirements(g_contextPtr->device, buffer, &memReqs);
        size = memReqs.size; //render doc complains but not vulkan ?

        const auto allocInfo = CreateAllocInfo(memReqs.size, GetMemoryType(g_contextPtr->physicalMemProps, memReqs, memProps));
        VkCheck(vkAllocateMemory(g_contextPtr->device, &allocInfo, nullptr, &memory)); //todo: allocate once for app and reuse memory pool
        VkCheck(vkBindBufferMemory(g_contextPtr->device, buffer, memory, 0));
    }

    ~Buffer()
    {
        //TODO: does not always work when Baking?
        if (buffer != nullptr){
            vkDestroyBuffer (g_contextPtr->device, buffer, nullptr);
            vkFreeMemory    (g_contextPtr->device, memory, nullptr);
            buffer = nullptr;
        }
    }
};

}//ns