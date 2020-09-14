//https://github.com/awwdev

#pragma once

#include "Vulkan/Meta/Context.hpp"
#include "Resources/CpuResources.hpp"

#include "Vulkan/_Old/Factories/UI/UI_RenderPass.hpp"
#include "Vulkan/_Old/Factories/UI/UI_Pipeline.hpp"
#include "Vulkan/_Old/Factories/UI/UI_Shader.hpp"
#include "Vulkan/_Old/Factories/UI/UI_UniformBuffer.hpp"

namespace rpg::vuk {

struct Resources_UI
{
    ImageArray fontImages;

    UI_PushConstants pushConsts;
    RenderPass       renderPass;
    Shader           shader;
    Pipeline         pipeline;
    UniformBuffer_Groups<gpu::UI_UniformData, gpu::UI_UBO_MAX_COUNT> ubo; //one group only

    void Create(res::HostResources& hostRes, VkCommandPool cmdPool)
    {
        fontImages.Create(hostRes.textures.monospaceFont, cmdPool);

        UI_CreateUniformBuffer    (ubo);
        UI_CreateShader           (shader, fontImages);
        UI_CreateRenderPass       (renderPass);
        UI_CreatePipeline         (pipeline, shader, renderPass, ubo);
    }

    void Recreate()
    {
        renderPass.Clear();
        pipeline.Clear();
        UI_CreateRenderPass       (renderPass);
        UI_CreatePipeline         (pipeline, shader, renderPass, ubo);
    }
};

}//NS