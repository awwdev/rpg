//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Abstraction/Meta/Context.hpp"
#include "gpu/Vulkan/Abstraction/Helper/Utils.hpp"

namespace rpg::gpu::vuk {

struct Buffer
{
    VkBuffer        buffer { VK_NULL_HANDLE };
    VkDeviceMemory  memory { VK_NULL_HANDLE };
    void*           memPtr;
    std::size_t     actualSize; //determined by GPU Memory requirements

    //? RAII

    void Create(const VkBufferUsageFlags usage, const std::size_t pSize, const VkMemoryPropertyFlags memProps)
    {
        const VkBufferCreateInfo bufferInfo {
            .sType                  = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext                  = nullptr,
            .flags                  = 0,
            .size                   = pSize,
            .usage                  = usage,
            .sharingMode            = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount  = 0,
            .pQueueFamilyIndices    = nullptr
        };
        VkCheck(vkCreateBuffer(g_contextPtr->device, &bufferInfo, nullptr, &buffer));

        VkMemoryRequirements memReqs;
        vkGetBufferMemoryRequirements(g_contextPtr->device, buffer, &memReqs);
        const auto allocInfo = MemoryAllocInfo(memReqs.size, MemoryType(g_contextPtr->physicalMemProps, memReqs, memProps));
        VkCheck(vkAllocateMemory(g_contextPtr->device, &allocInfo, nullptr, &memory));
        VkCheck(vkBindBufferMemory(g_contextPtr->device, buffer, memory, 0));

        actualSize = memReqs.size;
    }

    void Destroy()
    {
        vkDestroyBuffer (g_contextPtr->device, buffer, nullptr);
        vkFreeMemory    (g_contextPtr->device, memory, nullptr); //will unmap
        buffer = VK_NULL_HANDLE;
        memory = VK_NULL_HANDLE;
        memPtr = nullptr;
        actualSize = 0;
    }
    ~Buffer() { Destroy(); }

    //? STORE

    void Store(const void* data, const std::size_t size, const std::size_t offset = 0)
    {
        std::memcpy((char*)memPtr + offset, data, size);
    }
    
    void Map()   { VkCheck(vkMapMemory(g_contextPtr->device, memory, 0, VK_WHOLE_SIZE, 0, &memPtr)); }
    void Unmap() { vkUnmapMemory(g_contextPtr->device, memory); }

};

}//ns