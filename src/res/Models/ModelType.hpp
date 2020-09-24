//https://github.com/awwdev

#pragma once
#include "res/Models/ModelPrimitives.hpp"
#include "res/Models/Model.hpp"
#include "com/box/EnumMap.hpp"
#include "com/box/String.hpp"

namespace rpg::res {

enum class ModelType : idx_t
{
    PrimitiveCube, 
    PrimitiveQuad,  
    PrimitiveTriangle, 
    PrimitiveRing16,

    HARDCODED_ENUM_END,

    Sword,
    Stone, 
    Tree,
    Grass,

    ENUM_LAST,
    LOADED_ENUM_END = ENUM_LAST - 1 - HARDCODED_ENUM_END,
    LOADED_ENUM_BEGIN = HARDCODED_ENUM_END + 1,
};

const com::EnumMap<ModelType::HARDCODED_ENUM_END, Model> MAP_MODELS_HARDCODED
{
    { ModelType::PrimitiveCube,     MODEL_CUBE     },
    { ModelType::PrimitiveQuad,     MODEL_QUAD     },
    { ModelType::PrimitiveTriangle, MODEL_TRIANGLE },
    { ModelType::PrimitiveRing16,   MODEL_RING_16 },
};

const com::EnumMap<ModelType::LOADED_ENUM_END, com::String<30>, ModelType::LOADED_ENUM_BEGIN> MAP_MODELS_LOADED
{
    { ModelType::Sword, "res/Models/sword.txt" },
    { ModelType::Stone, "res/Models/stone.txt" },
    { ModelType::Tree,  "res/Models/tree.txt"  },
    { ModelType::Grass, "res/Models/grass.txt" },
};

}//ns