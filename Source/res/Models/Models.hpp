//https://github.com/awwdev

#pragma once
#include "res/Models/ModelLoader.hpp"
#include "com/Container/Array.hpp"
#include "gpu/RenderData/RenderData_General.hpp"

namespace rpg::res {

struct Models
{
    using Vertex = RenderData_General::Vertex;

    struct MeshRange
    {
        idx_t begin;
        idx_t end;
    };

    com::Array<Vertex, 1'000'000> allVertices;
    com::Array<MeshRange, 1'000>  meshRanges;

    void Load()
    {

    }

};

} //ns