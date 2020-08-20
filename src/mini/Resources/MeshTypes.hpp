//https://github.com/awwdev

#pragma once
#include "mini/Utils/Matrix.hpp"
#include "mini/Utils/PrimitiveMeshes.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/box/EnumMap.hpp"

namespace mini::res
{
    enum class MeshType
    {
        PrimitiveCube,
        PrimitiveQuad,
        PrimitiveTriangle,
        PrimitiveRing16, 
        Sword,

        //could add Custom1 and so on for user mod mesh later
        ENUM_END
        
    };

}//NS