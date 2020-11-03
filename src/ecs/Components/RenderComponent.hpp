//https://github.com/awwdev

#pragma once
#include "gpu/RenderData/RenderSettings.hpp"
#include "res/Mesh/MeshEnum.hpp"

namespace rpg::ecs {

struct RenderComponent
{
    res::MeshEnum meshEnum { res::MeshEnum::None };
    gpu::RenderSettings renderSettings;
};

}//ns