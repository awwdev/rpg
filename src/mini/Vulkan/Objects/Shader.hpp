//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"

#include "mini/Box/Array.hpp"
#include "mini/Memory/Allocator.hpp"

#include <fstream>

//TODO: remove VkArray and make it an actual helper class namespace mini::box

namespace mini::vk
{
    struct Refs
    {
        VkDevice device;
        Refs(Context& context) : device { context.device } {;}
    };

    struct Shader : Refs
    {
        enum Type { Vertex, Fragment, ENUM_END };

        VkShaderModule modules [Type::ENUM_END] { nullptr };

        box::Array<VkVertexInputBindingDescription, 10>   vertexBindings;
        box::Array<VkVertexInputAttributeDescription, 10> vertexAttributes;

        VkDescriptorPool descPool;
        box::Array<VkDescriptorSetLayoutBinding, 10> descSetLayoutBindings;
        VkArray<VkDescriptorSetLayout, 4> descSetLayouts;
        VkArray<VkDescriptorSet, 4> descSets;

        box::Array<VkSampler, 10> samplers;


        inline void AddDescriptor(
            const uint32_t binding, 
            const VkDescriptorType type,
            const uint32_t descCount,
            const VkShaderStageFlags stage)
        {
            descSetLayoutBindings.Append(binding, type, descCount, stage, nullptr);
        }

        inline void AddSampler()
        {
            //mapping? how to find the sampler u added
        }

        inline auto CreatePipelineInfo(const VkShaderStageFlagBits stage)
        {
            return VkPipelineShaderStageCreateInfo {
                .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .pNext  = nullptr,
                .flags  = 0,
                .stage  = stage,
                .module = [&]{ 
                    switch(stage){
                        case VK_SHADER_STAGE_VERTEX_BIT:   return modules[Type::Vertex];
                        case VK_SHADER_STAGE_FRAGMENT_BIT: return modules[Type::Fragment];
                    }
                }(),
                .pName  = "main",
                .pSpecializationInfo = nullptr 
            };
        }

        inline void AddVertexBinding(
            const uint32_t binding, 
            const uint32_t stride, 
            const VkVertexInputRate rate = VK_VERTEX_INPUT_RATE_VERTEX)
        {
            vertexBindings.Append(binding, stride, rate);
        }

        inline void AddVertexAttribute(
            const uint32_t binding, 
            const uint32_t location, 
            const VkFormat format, 
            const uint32_t offset)
        {
            vertexAttributes.Append(location, binding, format, offset);
        }

        template<std::size_t BUFFER_CAPACITY = 10000>
        inline void Load(const Type type, chars_t path)
        {
            std::ifstream file(path, std::ios::ate | std::ios::binary);
            mini::Assert(file.is_open(), "cannot open shader file");

            const uint32_t size = file.tellg();
            auto ptrBuffer = mini::mem::ClaimBlock<char[BUFFER_CAPACITY]>();
            file.seekg(std::ios::beg);
            file.read(*ptrBuffer, size);

            const VkShaderModuleCreateInfo moduleInfo {
                .sType      = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
                .pNext      = nullptr,
                .flags      = 0,
                .codeSize   = size,
                .pCode      = reinterpret_cast<const uint32_t*>(*ptrBuffer)
            };

            VK_CHECK(vkCreateShaderModule(device, &moduleInfo, nullptr, &modules[type]));
        }

        inline void Create(Context& context)
        {
            const VkDescriptorSetLayoutCreateInfo layoutSetInfo
            {
                .sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
                .pNext          = nullptr,
                .flags          = 0,
                .bindingCount   = descSetLayoutBindings.Count(),
                .pBindings      = descSetLayoutBindings.Data()
            };

            descSetLayouts.count = context.swapImages.count;
            FOR_VK_ARRAY(descSetLayouts, i)
                VK_CHECK(vkCreateDescriptorSetLayout(device, &layoutSetInfo, nullptr, &descSetLayouts[i]));

            VkArray<VkDescriptorPoolSize, 10> poolSizes;
            poolSizes.count = descSetLayoutBindings.Count();
            FOR_ARRAY(descSetLayoutBindings, i) {
                poolSizes[i] = {
                    .type = descSetLayoutBindings[i].descriptorType,
                    .descriptorCount = context.swapImages.count
                };
            }

            const VkDescriptorPoolCreateInfo poolInfo
             {
                .sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
                .pNext          = nullptr,
                .flags          = 0,
                .maxSets        = context.swapImages.count,
                .poolSizeCount  = poolSizes.count,
                .pPoolSizes     = poolSizes.data
            };
            VK_CHECK(vkCreateDescriptorPool(device, &poolInfo, nullptr, &descPool));

            const VkDescriptorSetAllocateInfo allocInfo 
            {
                .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
                .pNext              = nullptr,
                .descriptorPool     = descPool,
                .descriptorSetCount = descSetLayouts.count,
                .pSetLayouts        = descSetLayouts.data
            };
            descSets.count = descSetLayouts.count;
            VK_CHECK(vkAllocateDescriptorSets(device, &allocInfo, descSets.data));

            VkArray<VkWriteDescriptorSet, 4> writes { 0 };
            writes.count = context.swapImages.count;

            //! this is where it gets hairy since we need to store image link somehow
            //! also differentiation of image vs other ubo data

            //const VkDescriptorImageInfo imageInfo {
            //    .sampler        = samplers[0],
            //    .imageView      = image.view,
            //    .imageLayout    = image.layout
            //};  

            /*
            FOR_VK_ARRAY(writes, i)
            {
                for (uint32_t bindingNum = 0; bindingNum < ARRAY_COUNT(bindings); ++bindingNum)
                {
                    writes[i] = {
                        .sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                        .pNext              = nullptr,
                        .dstSet             = sets[i],
                        .dstBinding         = bindingNum,
                        .dstArrayElement    = 0,
                        .descriptorCount    = 1,
                        .descriptorType     = bindings[bindingNum].descriptorType,
                        .pImageInfo         = &imageInfo,//images.contains(bindingNum) ? &imageInfos[bindingNum] : nullptr,
                        .pBufferInfo        = nullptr,//ubos.contains(bindingNum) ? &ubos[bindingNum]->bufferInfos[i] : nullptr,
                        .pTexelBufferView   = nullptr
                    };
                }
            }
            vkUpdateDescriptorSets(device, writes.count, writes.data, 0, nullptr);
            */      
        }

        ~Shader()
        {
            vkDestroyDescriptorPool(device, descPool, nullptr);

            FOR_CARRAY(modules, i) {
                if (modules[i] != nullptr)
                    vkDestroyShaderModule(device, modules[i], nullptr);
            }       
            
            FOR_ARRAY(samplers, i)          vkDestroySampler(device, samplers[i], nullptr);
            FOR_VK_ARRAY(descSetLayouts, i) vkDestroyDescriptorSetLayout(device, descSetLayouts[i], nullptr);
        }
    };

    //? FACTORIES

    inline void CreateShader_Default(Context& context, Shader& shader)
    {
        shader.AddVertexBinding(0, sizeof(float) * 2);
        shader.AddVertexAttribute(0, 0, VK_FORMAT_R32_SFLOAT, sizeof(float) * 0);
        shader.AddVertexAttribute(0, 1, VK_FORMAT_R32_SFLOAT, sizeof(float) * 1);
        shader.AddDescriptor(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT);
        shader.Load(Shader::Type::Vertex,   "res/Shaders/default.vert.spv");
        shader.Load(Shader::Type::Fragment, "res/Shaders/default.frag.spv");
        shader.Create(context);
    }

}//ns