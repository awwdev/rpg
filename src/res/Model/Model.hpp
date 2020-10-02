//https://github.com/awwdev

#pragma once
#include "gpu/RenderData/RenderData_General.hpp"
#include "com/box/Array.hpp"
#include "com/Matrix.hpp"

namespace rpg::res {

constexpr idx_t MESH_COUNT_PER_MODEL = 10;
constexpr idx_t BONE_COUNT_PER_MODEL = 10;

using ModelVertex = gpu::RenderData_General::Vertex;

struct Material
{
    enum class Type { Opaque, Cutout } type;
};

struct Bone 
{
    com::Mat4f transform;
};

struct Mesh
{
    Material material;
    ModelVertex const* vertPtr; //points into vertex array of model resources struct
    idx_t vertCount;
};

struct Model
{
    com::Array<Mesh, MESH_COUNT_PER_MODEL> meshes;
    com::Array<Bone, BONE_COUNT_PER_MODEL> bones;
};

}//ns