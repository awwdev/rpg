//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"

namespace rpg::res {

enum class ModelType : idx_t
{
    PrimitiveCube, 
    PrimitiveQuad,  
    PrimitiveTriangle, 
    PrimitiveRing16,

    HARDCODED_ENUM_END,
    KEEP_ENUMERATION = HARDCODED_ENUM_END - 1, //DONT USE THIS

    Sword,
    Stone, 
    Tree,
    Grass,

    ENUM_END,
    LOADED_ENUM_END = ENUM_END - HARDCODED_ENUM_END,
    LOADED_ENUM_BEGIN = HARDCODED_ENUM_END,
};

}//ns