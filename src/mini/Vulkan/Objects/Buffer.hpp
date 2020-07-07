//https://github.com/awwdev

#pragma once

#include "mini/Vulkan/Context.hpp"
#include "mini/Utils/Structs.hpp"

namespace mini::vk
{
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
        ERR("no suitable memory type found!");
        return {};
    }


    struct Buffer
    {
        VkBuffer        buffer;
        VkDeviceMemory  memory;
        void*           memPtr;
        std::size_t     size;

        void Map()   { VK_CHECK(vkMapMemory(g_contextPtr->device, memory, 0, size, 0, &memPtr)); }
        void Unmap() { vkUnmapMemory(g_contextPtr->device, memory); }

        void Store(const void* const data, const size_t size, const size_t offset = 0)
        {
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
            VK_CHECK(vkCreateBuffer(g_contextPtr->device, &bufferInfo, nullptr, &buffer));

            //? MEMORY
            VkMemoryRequirements memReqs;
            vkGetBufferMemoryRequirements(g_contextPtr->device, buffer, &memReqs);
            size = memReqs.size; //render doc complains but not vulkan ?

            const auto allocInfo = CreateAllocInfo(memReqs.size, GetMemoryType(g_contextPtr->physicalMemProps, memReqs, memProps));
            VK_CHECK(vkAllocateMemory(g_contextPtr->device, &allocInfo, nullptr, &memory)); //todo: allocate once for app and reuse memory pool
            VK_CHECK(vkBindBufferMemory(g_contextPtr->device, buffer, memory, 0));

            Map();
        }

        ~Buffer()
        {
            Unmap();
            vkDestroyBuffer (g_contextPtr->device, buffer, nullptr);
            vkFreeMemory    (g_contextPtr->device, memory, nullptr);
        }
    };

}//ns

//? static buffer stuff:
//inline void copyBuffer(
//    VkQueue queue, 
//    VkDevice device, 
//    VkCommandPool cmdPool, 
//    VkBuffer src, 
//    VkBuffer dst, 
//    VkDeviceSize size) 
//{
//    auto cmdBuffer = utils::beginSingleTimeCommands(device, cmdPool);
//
//    const VkBufferCopy copyRegion {
//        .srcOffset = 0,
//        .dstOffset = 0,
//        .size = size
//    };
//    vkCmdCopyBuffer(cmdBuffer, src, dst, 1, &copyRegion);
//
//    utils::endSingleTimeCommands(device, cmdBuffer, cmdPool, queue);
//}
