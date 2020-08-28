//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"
#include "mini/Resources/HostResources.hpp"

#include "mini/Vulkan/Factories/UI/UI_RenderPass.hpp"
#include "mini/Vulkan/Factories/UI/UI_Pipeline.hpp"
#include "mini/Vulkan/Factories/UI/UI_Shader.hpp"
#include "mini/Vulkan/Factories/UI/UI_UniformBuffer.hpp"

namespace mini::vk {

struct Resources_UI
{
    ImageArray fontImages;

    UI_PushConstants pushConsts;
    RenderPass       renderPass;
    Shader           shader;
    Pipeline         pipeline;
    UniformBuffer_Groups<rendering::UI_UniformData, rendering::UI_UBO_MAX_COUNT> ubo; //one group only

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
        renderPass.~RenderPass();
        pipeline.~Pipeline();
        UI_CreateRenderPass       (renderPass);
        UI_CreatePipeline         (pipeline, shader, renderPass, ubo);
    }
};

}//NS