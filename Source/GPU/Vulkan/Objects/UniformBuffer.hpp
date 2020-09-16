//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Objects/Buffer.hpp"
#include "GPU/UboData.hpp"

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

    Buffer      buffer;
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
struct UniformBuffer
{
    static constexpr u32 MAX_COUNT  = MAX_COUNT_T;
    static constexpr u32 TOTAL_SIZE = sizeof(T) * MAX_COUNT_T;
    using  TYPE = T;

    Buffer      buffer;
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

    void Store(const T& data)
    {
        buffer.Store(&data, sizeof(T), 0);
        ++count;
    }

    void Reset()
    {
        count = 0;
    }

    ~UniformBuffer()
    {
        Clear();
    }

    void Clear()
    {
        buffer.Clear();
    }

};

}//ns