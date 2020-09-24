//https://github.com/awwdev

#pragma once
#include "res/Models/ModelPrimitives.hpp"
#include "res/Models/Model.hpp"
#include "com/box/EnumMap.hpp"
#include "com/box/String.hpp"

namespace rpg::res {

enum class ModelType_Hardcoded : idx_t
{
    PrimitiveCube, 
    PrimitiveQuad,  
    PrimitiveTriangle, 
    PrimitiveRing16,
    ENUM_END
};

const com::EnumMap<ModelType_Hardcoded::ENUM_END, Model> MAP_MODELS_HARDCODED
{
    { ModelType_Hardcoded::PrimitiveCube,     MODEL_CUBE     },
    { ModelType_Hardcoded::PrimitiveQuad,     MODEL_QUAD     },
    { ModelType_Hardcoded::PrimitiveTriangle, MODEL_TRIANGLE },
    { ModelType_Hardcoded::PrimitiveRing16,   MODEL_RING_16 },
};

enum class ModelType_Loaded : idx_t
{
    Sword,
    Stone, 
    Tree,
    Grass,
    ENUM_END
};

const com::EnumMap<ModelType_Loaded::ENUM_END, com::String<30>> MAP_MODELS_LOADED
{
    { ModelType_Loaded::Sword, "res/Models/sword.txt" },
    { ModelType_Loaded::Stone, "res/Models/stone.txt" },
    { ModelType_Loaded::Tree,  "res/Models/tree.txt"  },
    { ModelType_Loaded::Grass, "res/Models/grass.txt" },
};

}//ns