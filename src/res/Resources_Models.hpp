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

namespace rpg::res {

constexpr idx_t ALL_MODEL_VERTICES_MAX = 1'000'000;
constexpr idx_t MODELS_MAX = 1'000;

struct Resources_Models
{
    com::Array<Vertex, ALL_MODEL_VERTICES_MAX> allVertices;
    com::Array<ModelView, MODELS_MAX>          modelViews;

    void Load()
    {


    }

};

} //ns