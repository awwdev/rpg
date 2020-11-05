//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Abstraction/Objects/Pipeline.hpp"
#include "gpu/Vulkan/Abstraction/Objects/Shader.hpp"
#include "gpu/Vulkan/Abstraction/Objects/RenderPass.hpp"

namespace rpg::gpu::vuk {

template<auto... STAGES_T>
void CreatePipeline_Post(Pipeline& pipeline, Shader<STAGES_T...>& shader, RenderPass& renderPass)
{

}

}//ns