//https://github.com/awwdev

#pragma once
#include "gpu/RenderData/RenderData_General.hpp"
#include "com/box/Array.hpp"
#include "com/Matrix.hpp"

namespace rpg::res {

constexpr idx_t MESHES_MAX = 10;
constexpr idx_t BONES_MAX  = 10;

using Vertex = gpu::RenderData_General::Vertex;

enum class SurfaceType 
{
    VertexColor,
    AlphaCutout,
    Reflection,
};

struct MeshView
{
    SurfaceType surface;
    Vertex*     vertBegin;
    idx_t       vertCount;
};

struct Bone
{

};

struct ModelView 
{
    com::Array<MeshView, MESHES_MAX> meshViews;
    com::Array<Bone, BONES_MAX> bones;
};

}//ns