//https://github.com/awwdev

#pragma once

#include "mini/Vulkan/Context.hpp"
#include "mini/Utils/Structs.hpp"

namespace mini::vk
{
    inline VkMemoryAllocateInfo CreateAllocInfo(
        const VkDeviceSize& size,
        const uint32_t memTypeIndex)
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
        VkBuffer buffer;
        VkDeviceMemory memory;
        void* memPtr;
        std::size_t size;

        inline void Map()
        {
            VK_CHECK(vkMapMemory(context.device, memory, 0, size, 0, &memPtr));
            //VK_WHOLE_SIZE
        }

        inline void Unmap()
        {
            vkUnmapMemory(context.device, memory);
        }

 
        inline void Create(
            const VkBufferUsageFlags usage, 
            const std::size_t pSize, 
            const VkMemoryPropertyFlags memProps,
            const VkPhysicalDeviceMemoryProperties& physicalMemProps)
        {
            size   = pSize; 

            //? BUFFER

            const VkBufferCreateInfo bufferInfo
            {
                .sType                  = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
                .pNext                  = nullptr,
                .flags                  = 0,
                .size                   = size,
                .usage                  = usage,
                .sharingMode            = VK_SHARING_MODE_EXCLUSIVE,
                .queueFamilyIndexCount  = 0,
                .pQueueFamilyIndices    = nullptr
            };

            VK_CHECK(vkCreateBuffer(context.device, &bufferInfo, nullptr, &buffer));

            //? MEMORY

            VkMemoryRequirements memReqs;
            vkGetBufferMemoryRequirements(context.device, buffer, &memReqs);
            size = memReqs.size; //render doc complains but not vulkan ?

            const auto allocInfo = CreateAllocInfo(memReqs.size, GetMemoryType(physicalMemProps, memReqs, memProps));
            VK_CHECK(vkAllocateMemory(context.device, &allocInfo, nullptr, &memory)); //todo: allocate once for app and reuse memory pool
            VK_CHECK(vkBindBufferMemory(context.device, buffer, memory, 0));

            //if(memProps & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) return;
            Map();
        }

        inline void Store(const void* const data, const size_t size, const size_t offset = 0)
        {
            std::memcpy((char*)memPtr + offset, data, size);
        }

        ~Buffer()
        {
            Unmap();
            vkDestroyBuffer (context.device, buffer, nullptr);
            vkFreeMemory    (context.device, memory, nullptr);
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
