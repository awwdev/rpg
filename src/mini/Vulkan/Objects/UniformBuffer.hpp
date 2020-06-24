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
        VkDescriptorBufferInfo bufferInfo;
        VkDescriptorImageInfo  imageInfo;
    };


    struct UniformBuffer
    {
        Buffer buffer;
        const u32 MAX_COUNT;
        const VkDeviceSize ALIGNMENT;
        UniformInfo uniformInfo; //!set this in a "factory" method


        explicit UniformBuffer(
            Context& context, 
            const u32 maxCount) 
            : MAX_COUNT     { maxCount }
            , ALIGNMENT     { context.physicalProps.limits.minUniformBufferOffsetAlignment }
        {            
        }

        inline void Create(Context& context)
        {
            buffer.Create(
                context.device, 
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                ALIGNMENT * MAX_COUNT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                context.physicalMemProps
            );
        }

    };

}//ns