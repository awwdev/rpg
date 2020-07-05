//https://github.com/awwdev

#pragma once

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
        }

        void Store(const T* const ptr, const u32 pCount)
        {
            buffer.Store(ptr, pCount * sizeof(T));
            count = pCount;
        }

        template<u32 COUNT>
        void Store(const T (&arr)[COUNT])
        {
            buffer.Store(arr, COUNT * sizeof(T));
            count = COUNT;
        }

        void Store(const box::IArray<T>& arr)
        {
            buffer.Store(arr.dataPtr, arr.Count() * sizeof(T));
            count = arr.Count();
        }

    };



    
    struct UniformBuffer 
    {
        Buffer buffer;
        const u32 MAX_COUNT; //not used
        const u32 MAX_SIZE;
        const VkDeviceSize ALIGNMENT; //not used (needed for dynamic)
        UniformInfo uniformInfo; //!set this in a "factory" method


        explicit UniformBuffer(
            const u32 maxSize,
            const u32 alignment = 0, //not used
            const u32 maxCount = 0)//not used
            : MAX_COUNT     { maxCount  }
            , MAX_SIZE      { maxSize   }
            , ALIGNMENT     { alignment } //context.physicalProps.limits.minUniformBufferOffsetAlignment
        {            
        }

        void Create(Context& context)
        {
            buffer.Create(
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                MAX_SIZE,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
            );
        }

    };

}//ns