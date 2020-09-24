//https://github.com/awwdev

#pragma once
#include "gpu/RenderData/RenderData_General.hpp"

#include "res/Models/ModelLoader.hpp"
#include "res/Models/ModelPrimitives.hpp"
#include "res/Models/ModelType.hpp"
#include "res/Models/Model.hpp"

#include "com/box/Array.hpp"
#include "com/box/EnumMap.hpp"
#include "com/Range.hpp"
#include "dbg/Assert.hpp"

namespace rpg::res {

constexpr idx_t ALL_MODEL_VERTICES_MAX = 1'000'000;
constexpr idx_t MODEL_COUNT_MAX = 1'000;

struct Resources_Models
{
    com::Array<Vertex, ALL_MODEL_VERTICES_MAX> allVertices;
    com::Array<Model, MODEL_COUNT_MAX> models; //"views" into allVertices + meta data

    void Load()
    {
        //? EMPLACE HARDCODED PRIMITIVES (order is guaranteed)
        dbg::Assert((idx_t) ModelType_Hardcoded::ENUM_END == MODELS_HARDCODED.usedIndices.Count(), "mapping missing");
        for(idx_t i = 0; i < (idx_t) ModelType_Hardcoded::ENUM_END; ++i)
        {
            const auto& model = MODELS_HARDCODED.Get(i);
            const auto& mesh  = model.meshes[0]; //assumes one mesh
            allVertices.AppendArray(mesh.vertBegin, mesh.vertCount);   
        }
        //? LOAD EXTERNAL MODELS
    }

};

} //ns