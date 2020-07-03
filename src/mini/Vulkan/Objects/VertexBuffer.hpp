//https://github.com/awwdev

#pragma once

#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Objects/Buffer.hpp"

#include "mini/Box/Array.hpp"


namespace mini::vk
{
    /*
    struct VertexBuffer
    {
        //! count not actually needed since we can test host vertex buffer
        //(that we use to collect stuff and then we store it on gpu via this very class)
        //we could use count for bound checking though, but that is done auto by the host array ...

        Buffer vertexBuffer;
        Buffer indexBuffer;
        Buffer instanceBuffer;
        const std::size_t MAX_VERTEX_COUNT;
        const std::size_t MAX_INSTANCE_COUNT;

        box::Array<VkVertexInputBindingDescription, 10>   bindings;
        box::Array<VkVertexInputAttributeDescription, 10> attributes;

        explicit VertexBuffer(const std::size_t maxVertCount, const std::size_t maxInstCount)
            : MAX_VERTEX_COUNT   { maxVertCount }
            , MAX_INSTANCE_COUNT { maxInstCount }
        {;}

        inline void CreateDynamic(Context& context)
        {
            vertexBuffer.Create(
                context.device, 
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                sizeof(utils::Vertex) * MAX_VERTEX_COUNT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                context.physicalMemProps
            );

            indexBuffer.Create(
                context.device,
                VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
                sizeof(uint16_t) * MAX_VERTEX_COUNT, 
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                context.physicalMemProps
            );

            instanceBuffer.Create(
                context.device,
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
                sizeof(InstanceData_UI) * MAX_INSTANCE_COUNT, 
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                context.physicalMemProps
            );
        }

        inline void CreateStatic(VkDevice device, VkCommandPool cmdPool)
        {
            //TODO: do
        }

        //template<std::size_t N>
        //void Store(const utils::Vertex (&vertices)[N])
        //{
        //    buffer.Store(vertices, sizeof(vertices));
        //    count = N;
        //    indexCount = N + (N / 2);
        //}
//
        //void Store(const utils::Vertex* const vertices, const std::size_t pCount)
        //{
        //    buffer.Store(vertices, pCount * sizeof(utils::Vertex));
        //    count = pCount;
        //    indexCount = pCount + (pCount / 2);
        //}

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
    */

}//ns