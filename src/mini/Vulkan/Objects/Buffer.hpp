//https://github.com/awwdev

#pragma once

#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Resources.hpp"

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
        VkDevice device;

        VkBuffer buffer;
        VkDeviceMemory memory;
        void* memPtr;
        std::size_t size;


        inline void Map()
        {
            VK_CHECK(vkMapMemory(device, memory, 0, size, 0, &memPtr));
        }

        inline void Unmap()
        {
            vkUnmapMemory(device, memory);
        }


        inline void Create(
            Context& context, 
            const VkBufferUsageFlags usage, 
            const std::size_t pSize, 
            const VkMemoryPropertyFlags memProps)
        {
            device = context.device;
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

            VK_CHECK(vkCreateBuffer(device, &bufferInfo, nullptr, &buffer));

            //? MEMORY

            VkMemoryRequirements memReqs;
            vkGetBufferMemoryRequirements(device, buffer, &memReqs);
            
            const auto allocInfo = CreateAllocInfo(memReqs.size, GetMemoryType(context.physicalMemProps, memReqs, memProps));
            VK_CHECK(vkAllocateMemory(device, &allocInfo, nullptr, &memory));
            VK_CHECK(vkBindBufferMemory(device, buffer, memory, 0));

            //if(memProps & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) return;
            Map();
        }


        ~Buffer()
        {
            Unmap();
            vkDestroyBuffer(device, buffer, nullptr);
            vkFreeMemory(device, memory, nullptr);
        }
    };



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

}//ns