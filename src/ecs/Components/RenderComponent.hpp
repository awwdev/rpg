//https://github.com/awwdev

#pragma once
#include "gpu/RenderData/MaterialSettings.hpp"
#include "res/Mesh/MeshEnum.hpp"
#include "res/Mesh/MeshMaterialEnum.hpp"

namespace rpg::ecs {

struct RenderComponent
{
    res::MeshEnum mesh_enum { res::MeshEnum::Cube };
    res::MeshMaterialEnum material_enum { res::MeshMaterialEnum::General };
    gpu::MaterialSettings materialSettings;
};

}//ns