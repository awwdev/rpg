#pragma once

#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Objects/Buffer.hpp"

#include "mini/Box/Array.hpp"


namespace mini::vk
{
    struct VertexBuffer
    {
        Buffer buffer;
        //count
        //ranges, groups
        const std::size_t MAX_VERTEX_COUNT;

        box::Array<VkVertexInputBindingDescription, 10>   bindings;
        box::Array<VkVertexInputAttributeDescription, 10> attributes;

        explicit VertexBuffer(const std::size_t maxVertCount)
            : MAX_VERTEX_COUNT { maxVertCount }
        {;}

        inline void CreateDynamic(Context& context)
        {
            buffer.Create(
                context.device, 
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                sizeof(Vertex) * MAX_VERTEX_COUNT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                context.physicalMemProps
            );
        }

        inline void CreateStatic(VkDevice device, VkCommandPool cmdPool)
        {

        }

        template<std::size_t N>
        void Store(const Vertex (&vertices)[N])
        {
            buffer.Store(vertices, sizeof(vertices));
        }
    };

}//ns