//https://github.com/awwdev

#pragma once
#include "com/Utils.hpp"
#include "com/Matrix.hpp"
#include "gpu/RenderData/RenderData_General.hpp"

namespace rpg::res {
    
using MeshVertex = gpu::RenderData_General::Vertex;

static constexpr uint32_t MESHES_TOTAL = 100;
static constexpr uint32_t MESHES_VERTS_TOTAL = 1'000'000;

struct MeshVertexRange 
{ 
    uint32_t index;
    uint32_t count; 
};

}//ns