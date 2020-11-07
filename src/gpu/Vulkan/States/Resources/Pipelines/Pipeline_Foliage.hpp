//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Wrappers/Pipeline.hpp"
#include "gpu/Vulkan/Wrappers/Shader.hpp"
#include "gpu/Vulkan/Wrappers/RenderPass.hpp"

namespace rpg::gpu::vuk {

template<auto... STAGES_T>
void CreatePipeline_Foliage(Pipeline& pipeline, Shader<STAGES_T...>& shader, RenderPass& renderPass)
{

}

}//ns