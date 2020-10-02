//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"
#include "com/Matrix.hpp"
#include "gpu/RenderData/RenderData_General.hpp"

namespace rpg::res {
    
using MeshVertex = gpu::RenderData_General::Vertex;

static constexpr uint32_t MESHES_TOTAL = 100;
static constexpr uint32_t MESHES_VERTS_ALL = 1'000'000;

struct MeshView 
{ 
    idx_t begin;
    idx_t end; 
};

}//ns