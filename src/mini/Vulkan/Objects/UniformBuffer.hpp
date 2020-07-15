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

    //array is agnostic about layout of the array
    //groups does know about the layout of the array

    template<class T, u32 MAX_COUNT_T>
    struct UniformBuffer_Array
    {
        static constexpr u32 MAX_COUNT  = MAX_COUNT_T;
        static constexpr u32 TOTAL_SIZE = sizeof(T) * MAX_COUNT_T;
        using  TYPE = T;

        Buffer      buffer;
        UniformInfo info { .type = UniformInfo::Buffer };
        u32         count;
        //! COMPLETE UniformInfo IN A FACTORY METHOD

        void Create(VkMemoryPropertyFlags memFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
        {
            buffer.Create(
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                TOTAL_SIZE,
                memFlags
            );
            buffer.Map();
        }

        void Append(const T* const ptr, const u32 pCount)
        {
            buffer.Store(ptr, pCount * sizeof(T));
            count += pCount;
        }

        template<u32 COUNT>
        void Append(const T (&arr)[COUNT])
        {
            buffer.Store(arr, COUNT * sizeof(T));
            count += COUNT;
        }

        void Store(const box::IArray<T>& arr)
        {
            buffer.Store(arr.dataPtr, arr.Count() * sizeof(T));
            count += arr.Count();
        }

        void Clear()
        {
            count = 0;
        }

    };

    template<class T, u32 MAX_COUNT_T>
    struct UniformBuffer_Groups
    {
        static constexpr u32 MAX_COUNT  = MAX_COUNT_T;
        static constexpr u32 TOTAL_SIZE = sizeof(T) * MAX_COUNT_T;
        using  TYPE = T;

        Buffer      buffer;
        UniformInfo info { .type = UniformInfo::Buffer }; //!complete in factory

        u32 totalCount;
        u32 CurrentSize() const { return sizeof(T) * totalCount; }

        const box::IArray<rendering::UniformGroup>* groups = nullptr; //coming from rendergraph

        void Create(VkMemoryPropertyFlags memFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
        {
            buffer.Create(
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                TOTAL_SIZE,
                memFlags
            );
            buffer.Map();
        }

        //TODO: use max count constexpr inside a header an use on both rendering and vk 
        template<u32 N>
        void Store(const rendering::UniformDataGroups<T, N>& hostUBO)
        {
            buffer.Store(hostUBO.data.dataPtr, hostUBO.data.CurrentSize()); //no offset, all at once
            groups = &hostUBO.groups;
        }

        void Clear()
        {
            totalCount = 0;
            //groups.Clear();
        }

    };

}//ns