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
        const u32 MAX_COUNT; //not used
        const u32 MAX_SIZE;
        const VkDeviceSize ALIGNMENT; //not used (needed for dynamic)
        UniformInfo uniformInfo; //!set this in a "factory" method


        explicit UniformBuffer(
            Context& context, 
            const u32 maxSize,
            const u32 alignment = 0, //not used
            const u32 maxCount = 0)//not used
            : MAX_COUNT     { maxCount  }
            , MAX_SIZE      { maxSize   }
            , ALIGNMENT     { alignment } //context.physicalProps.limits.minUniformBufferOffsetAlignment
        {            
        }

        inline void Create(Context& context)
        {
            buffer.Create(
                context.device, 
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                MAX_SIZE,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                context.physicalMemProps
            );
        }

    };

}//ns