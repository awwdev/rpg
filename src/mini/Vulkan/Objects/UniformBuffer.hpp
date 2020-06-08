//https://github.com/awwdev

#pragma once

#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Objects/Buffer.hpp"

#include "mini/Box/Array.hpp"


namespace mini::vk
{
    struct UboData_Default //TODO: move outside, its a gpu api agnostic  struct
    {
        bool useTexture = false;   
    };


    struct UniformBuffer
    {
        Buffer buffer;
        const u32 MAX_COUNT;
        const VkDeviceSize ALIGNMENT;
        const VkDescriptorType DESC_TYPE;
        const VkShaderStageFlags SHADER_STAGE;

        VkDescriptorBufferInfo descBufferInfo { //modify if needed
            .buffer = buffer.buffer,
            .offset = 0,
            .range  = VK_WHOLE_SIZE
        };


        explicit UniformBuffer(
            Context& context, 
            const u32 maxCount, 
            const VkDescriptorType descType, 
            const VkShaderStageFlags stage) 
            : MAX_COUNT     { maxCount }
            , ALIGNMENT     { context.physicalProps.limits.minUniformBufferOffsetAlignment }
            , DESC_TYPE     { descType }
            , SHADER_STAGE  { stage }
        {;}

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

        inline auto GetLayoutBinding(const uint32_t binding) const
        {
            return VkDescriptorSetLayoutBinding {
                .binding            = binding,
                .descriptorType     = DESC_TYPE,
                .descriptorCount    = 1,
                .stageFlags         = SHADER_STAGE,
                .pImmutableSamplers = nullptr,
            }; 
        }
    };


    

    //TODO: temp move (find better place?) PIPELINE?
    inline void CreateDescriptors(Context& context)
    {
        /*
        const VkDescriptorSetLayoutCreateInfo setLayoutInfo
        {
            .sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext          = nullptr,
            .flags          = 0,
            .bindingCount   = setLayoutBindings.Count(),
            .pBindings      = setLayoutBindings.Data()
        };

        for(uint32_t i = 0; i < context.swapImages.count; ++i)
            VK_CHECK(vkCreateDescriptorSetLayout(context.device, &setLayoutInfo, nullptr, &setLayouts.AppendReturn()));

        box::Array<VkDescriptorPoolSize, 10> poolSizes;
        for(uint32_t i = 0; i < setLayoutBindings.Count(); ++i) {
            poolSizes.Append(VkDescriptorPoolSize{
                .type = setLayoutBindings[i].descriptorType,
                .descriptorCount = context.swapImages.count
            });
        }

        const VkDescriptorPoolCreateInfo poolInfo {
            .sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext          = nullptr,
            .flags          = 0,
            .maxSets        = context.swapImages.count,// * poolSizes.Count(), //! needed ? 
            .poolSizeCount  = poolSizes.Count(),
            .pPoolSizes     = poolSizes.Data()
        };
        VK_CHECK(vkCreateDescriptorPool(context.device, &poolInfo, nullptr, &descPool));

        const VkDescriptorSetAllocateInfo allocInfo {
            .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext              = nullptr,
            .descriptorPool     = descPool,
            .descriptorSetCount = setLayouts.Count(),
            .pSetLayouts        = setLayouts.Data()
        };
        sets.count = setLayouts.Count();
        VK_CHECK(vkAllocateDescriptorSets(context.device, &allocInfo, sets.Data()));

        box::Array<VkWriteDescriptorSet, 10> writes;
        for(auto i = 0; i < context.swapImages.count; ++i)
        {
            FOR_ARRAY(setLayoutBindings, j)
            {
                writes.Append(VkWriteDescriptorSet{
                    .sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                    .pNext              = nullptr,
                    .dstSet             = sets[i],
                    .dstBinding         = j,
                    .dstArrayElement    = 0,
                    .descriptorCount    = 1,
                    .descriptorType     = setLayoutBindings[j].descriptorType,
                    .pImageInfo         = imageInfos.GetValueOptional(j),
                    .pBufferInfo        = bufferInfos.GetValueOptional(j),
                    .pTexelBufferView   = nullptr
                });
            }
        }
        vkUpdateDescriptorSets(context.device, writes.Count(), writes.Data(), 0, nullptr);   
        */
    }

}//ns