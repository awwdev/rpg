//https://github.com/awwdev

#pragma once
#include "gpu/RenderData/RenderData_General.hpp"
#include "com/box/Array.hpp"
#include "com/Matrix.hpp"

namespace rpg::res {

constexpr idx_t MESH_VIEWS_MAX = 10;
constexpr idx_t BONES_MAX  = 10;

using ModelVertex = gpu::RenderData_General::Vertex;

enum class Blending 
{
    Opaque,
    Alpha,
};

struct MeshView
{
    Blending           blending;
    ModelVertex const* vertPtr;
    idx_t              vertCount;
};

struct Bone
{
    com::Mat4f transform;
    //vertices will have data on weights
};

struct ModelView
{
    com::Array<MeshView, MESH_VIEWS_MAX> meshViews;
    com::Array<Bone, BONES_MAX> bones;
};

}//ns