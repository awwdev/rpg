//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Abstraction/Objects/Pipeline.hpp"
#include "gpu/Vulkan/Abstraction/Objects/Shader.hpp"

namespace rpg::gpu::vuk {

template<auto... STAGES_T>
void CreatePipeline_GUI(Shader<STAGES_T...>& shader)
{

}

}//ns