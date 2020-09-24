//https://github.com/awwdev

#pragma once

namespace rpg::res {

enum class ModelType_Hardcoded : idx_t
{
    PrimitiveCube, 
    PrimitiveQuad,  
    PrimitiveTriangle, 
    PrimitiveRing16,

    ENUM_END
};

enum class ModelType_Loaded : idx_t
{
    Sword = (s32)ModelType_Hardcoded::ENUM_END,
    Stone, 
    Tree,
    Grass,

    ENUM_END
};

}//ns