//https://github.com/awwdev

#pragma once

#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Objects/Buffer.hpp"

#include "mini/Box/Array.hpp"


namespace mini::vk
{
    struct VertexBuffer
    {
        Buffer buffer;
        u32    count = 0;
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
            //TODO: do
        }

        template<std::size_t N>
        void Store(const Vertex (&vertices)[N])
        {
            buffer.Store(vertices, sizeof(vertices));
            count = N;
        }

        void Store(const Vertex* const vertices, const std::size_t pCount)
        {
            buffer.Store(vertices, pCount * sizeof(Vertex));
            count = pCount;
        }

        //TODO: StoreGroup / Range 
        //TODO: when drawing (cmd draw) wwe actually ask the vbo how much verts he has (count, not capacity)
    };

    inline auto CreatePipelineVertexInputInfo(VertexBuffer& vb) -> VkPipelineVertexInputStateCreateInfo
    {
        return {
            .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .pNext                           = nullptr,
            .flags                           = 0,
            .vertexBindingDescriptionCount   = vb.bindings.Count(),
            .pVertexBindingDescriptions      = vb.bindings.Data(),
            .vertexAttributeDescriptionCount = vb.attributes.Count(),
            .pVertexAttributeDescriptions    = vb.attributes.Data()
        };
    }

}//ns