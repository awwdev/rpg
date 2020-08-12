//https://github.com/awwdev

#pragma once
#include "mini/Utils/Structs.hpp"
#include "mini/Box/Array.hpp"
#include "mini/ECS/ECS.hpp"
#include "mini/Rendering/Cameras.hpp"
#include "mini/Resources/Terrain/Terrain_Quadrant.hpp"

#include <fstream>

namespace mini::res2 {

    template<auto LENGTH, auto QUAD_COUNT>
    struct Quadrant
    {

    };

    template<
        auto QUADRANT_COUNT,
        auto QUADRANT_LENGTH,
        auto QUADRANT_QUAD_COUNT>
    struct Terrain
    {
        using QUADRANT_T = Quadrant<QUADRANT_LENGTH, QUADRANT_QUAD_COUNT>;
        
        QUADRANT_T quadrants [QUADRANT_COUNT][QUADRANT_COUNT];


    };

} //ns