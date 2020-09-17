//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Objects/Buffer.hpp"

namespace rpg::gpu::vuk {

template<class T, idx_t MAX_COUNT_T>
struct UniformBuffer
{
    static constexpr idx_t MAX_COUNT = MAX_COUNT_T;
    static constexpr idx_t BYTE_SIZE = MAX_COUNT_T * sizeof(T);

    Buffer  cpuBuffer;
    Buffer  gpuBuffer; //possibility to bake into device local memory
    Buffer* activeBuffer;

    idx_t count;
    auto CurrentByteSize() const { return count * sizeof(T); }
    
    void Create()
    {
        cpuBuffer.Create(
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            BYTE_SIZE,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );
        cpuBuffer.Map();
        activeBuffer = &cpuBuffer;
    }

    void Bake(VkCommandPool cmdPool)
    {
        if (gpuBuffer.buffer) 
            gpuBuffer.Clear();

        gpuBuffer.Create(
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            CurrentByteSize(),
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        gpuBuffer.Map();
        CopyBuffer(cmdPool, cpuBuffer, gpuBuffer);
        gpuBuffer.Unmap();

        activeBuffer = &gpuBuffer;
    }

    void Clear()
    {
        if (cpuBuffer.buffer) cpuBuffer.Clear();
        if (gpuBuffer.buffer) gpuBuffer.Clear();
        activeBuffer = nullptr;
    }
    ~UniformBuffer() { Clear(); }

};

}//ns