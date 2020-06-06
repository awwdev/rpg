//https://github.com/awwdev

#pragma once

#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Objects/Buffer.hpp"

#include "mini/Box/Array.hpp"


namespace mini::vk
{
    template<class ELEMENT>
    struct UniformBuffer
    {
        Buffer buffer;
        const u32 MAX_COUNT;

        explicit UniformBuffer(const u32 maxCount) : MAX_COUNT { maxCount }
        {;}

        inline void Create(Context& context)
        {
            //TODO: get multiple of 256 (or gpu ubo min size)
            buffer.Create(
                context.device, 
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                //sizeof(ELEMENT) * MAX_COUNT,
                256 * 2,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                context.physicalMemProps
            );
        }

        //whole buffer store intended
        //void Store(const ELEMENT* content, const std::size_t size)
        //{
        //    buffer.Store(&content, sizeof(content));
        //}

    };


}//ns