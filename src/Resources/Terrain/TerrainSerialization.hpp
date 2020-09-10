
//https://github.com/awwdev

#pragma once
#include "Utils/Types.hpp"
#include "Resources/Terrain/TerrainQuadrant.hpp"

#include <fstream>

namespace rpg::res {

template<bool DoSave, class QUADRANT_T, auto QUADRANT_COUNT>
void WriteOrRead(QUADRANT_T (&quadrants)[QUADRANT_COUNT][QUADRANT_COUNT])
{
    using namespace use;

    char path[] { "res/Terrain/txx" };

    for(idx_t z = 0; z < QUADRANT_COUNT; ++z) {
    for(idx_t x = 0; x < QUADRANT_COUNT; ++x) {
        
        const auto& quadrant = quadrants[z][x];
        path[13] = (char)(48 + z);
        path[14] = (char)(48 + x);
        
        auto file = [&] {
            if constexpr (DoSave) return std::ofstream(path, std::ios::binary);
            else                  return std::ifstream(path, std::ios::binary);
        }();
        
        if (!file) dbg::LogError("cannot open file");

        if constexpr (DoSave)  file.write((const char*)quadrant.verts, sizeof(use::Common_Vertex) * quadrant.VERT_COUNT_TOTAL);
        else                   file.read((char*)quadrant.verts, sizeof(use::Common_Vertex) * quadrant.VERT_COUNT_TOTAL);
    }}
}

template<class QUADRANT_T, auto QUADRANT_COUNT>
void SaveTerrain(const QUADRANT_T (&quadrants)[QUADRANT_COUNT][QUADRANT_COUNT])
{
    dbg::LogInfo("saving terrain");
    WriteOrRead<true>(quadrants);
}

template<class QUADRANT_T, auto QUADRANT_COUNT>
void LoadTerrain(QUADRANT_T (&quadrants)[QUADRANT_COUNT][QUADRANT_COUNT])
{
    dbg::LogInfo("loading terrain");
    WriteOrRead<false>(quadrants);
}

} //ns