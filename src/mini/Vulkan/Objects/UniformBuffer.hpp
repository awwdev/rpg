//https://github.com/awwdev

#pragma once

#include "mini/Rendering/UboData.hpp"
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Objects/Buffer.hpp"

#include "mini/Box/Array.hpp"


namespace mini::vk
{
    struct UniformInfo
    {
        enum Type { Buffer, Image } type;
        VkDescriptorSetLayoutBinding layout; //includes binding
        union {
            VkDescriptorBufferInfo bufferInfo;
            VkDescriptorImageInfo  imageInfo;
        };
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

        u32 count;
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
        void Store(const rendering::UniformDataGroups<T, MAX_COUNT_T, GROUPS_COUNT>& hostUBO)
        {
            buffer.Store(hostUBO.data.dataPtr, hostUBO.data.CurrentSize()); //no offset, all at once
            count = hostUBO.data.count;
        }

        void Clear()
        {
            count = 0;
        }

    };

}//ns