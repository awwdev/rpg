 //https://github.com/awwdev

#pragma once
#include "com/Utils.hpp"
#include "com/Matrix.hpp"
#include "res/Terrain/Terrain2.hpp"

namespace rpg::res2 {
 
inline void PrintTerrain(Terrain const& terrain)
{
    for(auto z = 0; z < terrain.QUADRANT_COUNT; ++z) {
    for(auto x = 0; x < terrain.QUADRANT_COUNT; ++x) {
        dbg::LogColor(dbg::ConsoleColors::BrightCyan, "quadrant ", z, '|', x);
            Print2DArray(terrain.quadrants[z][x].vertices);
            Print1DArray(terrain.quadrants[z][x].indices);
    }}
}

} //ns