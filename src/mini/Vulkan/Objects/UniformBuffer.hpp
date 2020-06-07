//https://github.com/awwdev

#pragma once

#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Objects/Buffer.hpp"

#include "mini/Box/Array.hpp"


namespace mini::vk
{
    struct UboData_Default
    {
        bool useTexture = false;   
        //! alignment 
    };

    struct UniformBuffer
    {
        Buffer buffer;
        const u32 MAX_COUNT;
        const VkDeviceSize ALIGNMENT;

        explicit UniformBuffer(Context& context, const u32 maxCount) 
            : MAX_COUNT { maxCount }
            , ALIGNMENT { context.physicalProps.limits.minUniformBufferOffsetAlignment }
        {;}

        inline void Create(Context& context)
        {
            buffer.Create(
                context.device, 
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                ALIGNMENT * 2,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                context.physicalMemProps
            );
        }

        //TODO: store method, more specific than buffer.Store(raw) ?
    };

}//ns