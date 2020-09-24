//https://github.com/awwdev

#pragma once
#include "gpu/RenderData/RenderData_General.hpp"
#include "com/box/Array.hpp"
#include "com/Matrix.hpp"

namespace rpg::res {

constexpr idx_t MESHES_MAX = 10;
constexpr idx_t BONES_MAX  = 10;

using Vertex = gpu::RenderData_General::Vertex;

enum class Blending 
{
    Opaque,
    Alpha,
};

struct Mesh
{
    Blending       blending;
    Vertex const*  vertBegin;
    idx_t          vertCount;
    //additional material ubo data
};

struct Bone
{
    com::Mat4f transform;
};

struct Model 
{
    com::Array<Mesh, MESHES_MAX> meshes;
    com::Array<Bone, BONES_MAX>  bones;
};

}//ns