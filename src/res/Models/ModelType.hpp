//https://github.com/awwdev

#pragma once
#include "res/Models/ModelPrimitives.hpp"
#include "res/Models/Model.hpp"
#include "com/box/EnumMap.hpp"

namespace rpg::res {

enum class ModelType_Hardcoded : idx_t
{
    PrimitiveCube, 
    PrimitiveQuad,  
    PrimitiveTriangle, 
    PrimitiveRing16,

    ENUM_END
};

const com::EnumMap<ModelType_Hardcoded::ENUM_END, Model> MODELS_HARDCODED
{
    { ModelType_Hardcoded::PrimitiveCube,     MODEL_CUBE     },
    { ModelType_Hardcoded::PrimitiveQuad,     MODEL_QUAD     },
    { ModelType_Hardcoded::PrimitiveTriangle, MODEL_TRIANGLE },
    { ModelType_Hardcoded::PrimitiveRing16,   MODEL_RING_16 },
};

enum class ModelType_Loaded : idx_t
{
    ENUM_BEGIN = (idx_t) ModelType_Hardcoded::ENUM_END - 1,

    Sword,
    Stone, 
    Tree,
    Grass,

    ENUM_END
};

}//ns