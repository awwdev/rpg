
//https://github.com/awwdev

#pragma once
#include "mini/Utils/Structs.hpp"
#include "mini/Box/Array.hpp"
#include "mini/ECS/ECS.hpp"
#include "mini/Rendering/Cameras.hpp"
#include "mini/Resources/Terrain/TerrainQuadrant.hpp"

#include <fstream>

namespace mini::res {

template<class QUADRANT_T, auto QUADRANT_COUNT>
void SaveTerrain(const QUADRANT_T (&quadrants)[QUADRANT_COUNT][QUADRANT_COUNT])
{
    using namespace utils;
    dbg::LogInfo("saving terrain");

    for(idx_t z = 0; z < QUADRANT_COUNT; ++z) {
    for(idx_t x = 0; x < QUADRANT_COUNT; ++x) {

        char path[] { "res/xxx" };
        path[4] = 't';
        path[5] = (char)(48 + z);
        path[6] = (char)(48 + x);
        
        std::ofstream file(path, std::ios::binary);
        if (!file) dbg::LogError("cannot open file");

        const auto& quadrant = quadrants[z][x];
        file.write((const char*)quadrant.verts, sizeof(utils::Common_Vertex) * quadrant.VERT_COUNT_TOTAL);
    }}
}

template<class QUADRANT_T, auto QUADRANT_COUNT>
void LoadTerrain(QUADRANT_T (&quadrants)[QUADRANT_COUNT][QUADRANT_COUNT])
{
    using namespace utils;
    dbg::LogInfo("loading terrain");

    for(idx_t z = 0; z < QUADRANT_COUNT; ++z) {
    for(idx_t x = 0; x < QUADRANT_COUNT; ++x) {

        char path[] { "res/xxx" };
        path[4] = 't';
        path[5] = (char)(48 + z);
        path[6] = (char)(48 + x);
            
        std::ifstream file(path, std::ios::binary);
        if (!file) dbg::LogError("cannot open file");

        const auto& quadrant = quadrants[z][x];
        file.read((char*)quadrant.verts, sizeof(utils::Common_Vertex) * quadrant.VERT_COUNT_TOTAL);
    }}
}

} //ns