//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Meta/Context.hpp"
#include "gpu/Vulkan/Objects/Buffer.hpp"
#include "com/box/Array.hpp"

namespace rpg::gpu::vuk {

enum class BufferExtType 
{ 
    UniformBuffer, 
    VertexBuffer,
    StorageBuffer,
    IndexBuffer,
};

template<BufferExtType BUFFER_EXT_TYPE, class ELEMENT_TYPE, idx_t MAX_COUNT_T>
struct BufferExt
{
    static constexpr idx_t MAX_COUNT = MAX_COUNT_T;
    static constexpr idx_t BYTE_SIZE = MAX_COUNT_T * sizeof(ELEMENT_TYPE);

    Buffer  cpuBuffer;
    Buffer  gpuBuffer; //possibility to (re)bake into device local memory
    Buffer* activeBuffer;

    idx_t count = 0;
    auto CurrentByteSize() const { return count * sizeof(ELEMENT_TYPE); }
    
    static constexpr auto BUFFER_USAGE = [] {
        switch(BUFFER_EXT_TYPE) 
        {
            case BufferExtType::UniformBuffer: return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            case BufferExtType::VertexBuffer:  return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            case BufferExtType::StorageBuffer: return VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
            case BufferExtType::IndexBuffer:   return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            default: dbg::Assert(false, "BufferExtType missing");
        }
    }();

    static constexpr auto DESCRIPTOR_TYPE = [] {
        switch(BUFFER_EXT_TYPE) 
        {
            case BufferExtType::UniformBuffer: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            case BufferExtType::StorageBuffer: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        }
    }();

    //? RAII

    void Create()
    {
        cpuBuffer.Create(
            BUFFER_USAGE | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            BYTE_SIZE,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );
        cpuBuffer.Map();
        activeBuffer = &cpuBuffer;
    }

    void Reset(const uint32_t idx = 0)
    {
        count = idx;
    }

    void Destroy()
    {
        if (cpuBuffer.buffer) cpuBuffer.Destroy();
        if (gpuBuffer.buffer) gpuBuffer.Destroy();
        activeBuffer = nullptr;
        Reset();
    }
    ~BufferExt() { Destroy(); }

    //? STORE

    void Append(const ELEMENT_TYPE& element)
    {
        activeBuffer->Store(&element, sizeof(ELEMENT_TYPE), CurrentByteSize());
        ++count;
    }

    void Append(const ELEMENT_TYPE* arr, const std::size_t pCount)
    {
        activeBuffer->Store(arr, sizeof(ELEMENT_TYPE) * pCount, CurrentByteSize());
        count += pCount;
    }

    template<auto ARR_COUNT>
    void Append(const ELEMENT_TYPE (&arr) [ARR_COUNT])
    {
        activeBuffer->Store(arr, sizeof(ELEMENT_TYPE) * ARR_COUNT, CurrentByteSize());
        count += ARR_COUNT;
    }

    template<auto N>
    void Append(const com::Array<ELEMENT_TYPE, N>& arr)
    {
        activeBuffer->Store(arr.Data(), arr.CurrentByteSize(), CurrentByteSize());
        count += arr.Count();
    }

    //? BAKE

    void Bake(VkCommandPool cmdPool)
    {
        if (gpuBuffer.buffer) {
            VkCheck(vkQueueWaitIdle(g_contextPtr->queue));
            gpuBuffer.Destroy();
        }

        gpuBuffer.Create(
            BUFFER_USAGE | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            CurrentByteSize(),
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        CopyBuffer(cmdPool, cpuBuffer.buffer, gpuBuffer.buffer, CurrentByteSize());
        activeBuffer = &gpuBuffer;
    }

};

template<typename ELEMENT_TYPE, idx_t MAX_COUNT_T>
using UniformBuffer = BufferExt<BufferExtType::UniformBuffer, ELEMENT_TYPE, MAX_COUNT_T>;

template<typename ELEMENT_TYPE, idx_t MAX_COUNT_T>
using StorageBuffer = BufferExt<BufferExtType::StorageBuffer, ELEMENT_TYPE, MAX_COUNT_T>;

template<typename ELEMENT_TYPE, idx_t MAX_COUNT_T>
using VertexBuffer  = BufferExt<BufferExtType::VertexBuffer, ELEMENT_TYPE, MAX_COUNT_T>;

template<typename ELEMENT_TYPE, idx_t MAX_COUNT_T>
using IndexBuffer  = BufferExt<BufferExtType::IndexBuffer, ELEMENT_TYPE, MAX_COUNT_T>;

}//ns