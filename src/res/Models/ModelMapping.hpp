//https://github.com/awwdev

#pragma once
#include "res/Models/Model.hpp"
#include "res/Models/ModelPrimitives.hpp"
#include "res/Models/ModelType.hpp"
#include "com/box/EnumMap.hpp"
#include "com/box/StringMap.hpp"
#include "com/box/String.hpp"
#include "ecs/Components/Meta/ComponentParsing.hpp"

namespace rpg::res {

//? HARDCODED MODEL DATA

const com::EnumMap<ModelType::HARDCODED_ENUM_END, Model> 
MAP_MODELS_HARDCODED
{
    { ModelType::PrimitiveCube,     MODEL_CUBE     },
    { ModelType::PrimitiveQuad,     MODEL_QUAD     },
    { ModelType::PrimitiveTriangle, MODEL_TRIANGLE },
    { ModelType::PrimitiveRing16,   MODEL_RING_16 },
};

//? PATHS TO LOAD MODELS

constexpr auto MODEL_PATH_STR_MAX = 30;
const com::EnumMap<ModelType::LOADED_ENUM_END, com::String<MODEL_PATH_STR_MAX>, ModelType::LOADED_ENUM_BEGIN> 
MAP_MODELS_LOADED
{
    { ModelType::Sword, "res/Models/sword.txt" },
    { ModelType::Stone, "res/Models/stone.txt" },
    { ModelType::Tree,  "res/Models/tree.txt"  },
    { ModelType::Grass, "res/Models/grass.txt" },
};

//? MODEL NAMES FOR PARSING

constexpr auto MODEL_NAME_STR_MAX = 100;
const com::EnumMap<ModelType::ENUM_END, com::String<MODEL_NAME_STR_MAX>> 
MAP_MODEL_ENUM_TO_STR
{
    { ModelType::PrimitiveCube,      "PrimitiveCube" },
    { ModelType::PrimitiveQuad,      "PrimitiveQuad" },
    { ModelType::PrimitiveTriangle,  "PrimitiveTriangle" },
    { ModelType::PrimitiveRing16,    "PrimitiveRing16" },
    { ModelType::Sword,              "Sword" },
    { ModelType::Grass,              "Grass" },
    { ModelType::Stone,              "Stone" },
    { ModelType::Tree,               "Tree" },
};
const auto MAP_MODEL_STR_TO_ENUM = com::StringMapFromEnumMap<ModelType, MODEL_NAME_STR_MAX>(MAP_MODEL_ENUM_TO_STR);

inline ModelType GetModelType(const ecs::ComponentDataString& str)
{
    const ModelType* modelType = MAP_MODEL_STR_TO_ENUM.GetOptional(str.data);
    dbg::Assert(modelType, "no model type found");
    return *modelType;
}

}//ns