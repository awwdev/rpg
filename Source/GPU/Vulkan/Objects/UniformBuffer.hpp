//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Objects/Buffer.hpp"

namespace rpg::gpu::vuk {

template<class T, idx_t MAX_COUNT_T>
struct UniformBuffer2
{
    static constexpr idx_t MAX_COUNT = MAX_COUNT_T;
    static constexpr idx_t BYTE_SIZE = MAX_COUNT_T * sizeof(T);

    Buffer2  cpuBuffer;
    Buffer2  gpuBuffer; //possibility to (re)bake into device local memory
    Buffer2* activeBuffer;

    idx_t count;
    auto CurrentByteSize() const { return count * sizeof(T); }
    
    //? RAII

    void Create()
    {
        cpuBuffer.Create(
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            BYTE_SIZE,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );
        cpuBuffer.Map();
        activeBuffer = &cpuBuffer;
    }

    void Clear()
    {
        if (cpuBuffer.buffer) cpuBuffer.Clear();
        if (gpuBuffer.buffer) gpuBuffer.Clear();
        activeBuffer = nullptr;
        count = 0;
    }
    ~UniformBuffer2() { Clear(); }

    //? STORE

    void StoreElement(const T& element)
    {
        activeBuffer->Store(&element, sizeof(T), CurrentByteSize());
        ++count;
    }

    void StoreArray(const T* arr, const std::size_t pCount)
    {
        activeBuffer->Store(&arr, sizeof(T) * pCount, CurrentByteSize());
        count += pCount;
    }

    template<std::size_t ARR_COUNT>
    void StoreArray(const T (&arr) [ARR_COUNT])
    {
        activeBuffer->Store(&arr, sizeof(T) * ARR_COUNT, CurrentByteSize());
        count += ARR_COUNT;
    }

    //? BAKE

    void Bake(VkCommandPool cmdPool)
    {
        if (gpuBuffer.buffer) {
            VkCheck(vkQueueWaitIdle(g_contextPtr->queue));
            gpuBuffer.Clear();
        }

        gpuBuffer.Create(
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            CurrentByteSize(),
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        CopyBuffer(cmdPool, cpuBuffer.buffer, gpuBuffer.buffer, CurrentByteSize());
        activeBuffer = &gpuBuffer;
    }

};

}//ns