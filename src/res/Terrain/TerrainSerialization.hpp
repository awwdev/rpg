
//https://github.com/awwdev

#pragma once
#include "com/Utils.hpp"
#include "res/Terrain/TerrainQuadrant.hpp"

#include <fstream>

namespace rpg::res {

template<bool DoSave, class QUADRANT_T, auto QUADRANT_COUNT>
void WriteOrRead(QUADRANT_T (&quadrants)[QUADRANT_COUNT][QUADRANT_COUNT])
{
    using namespace com;

    char path[] { "out/tmp/txx" }; //TODO: replace with current working dir

    for(idx_t z = 0; z < QUADRANT_COUNT; ++z) {
    for(idx_t x = 0; x < QUADRANT_COUNT; ++x) {
        
        const auto& quadrant = quadrants[z][x];
        path[9] = (char)(48 + z);
        path[10] = (char)(48 + x);
        
        auto file = [&] {
            if constexpr (DoSave) return std::ofstream(path, std::ios::binary);
            else                  return std::ifstream(path, std::ios::binary);
        }();
        
        dbg::Assert(file.is_open(), "cannot open file");

        if constexpr (DoSave)  file.write((const char*)quadrant.verts, sizeof(gpu::RenderData_General::Vertex) * quadrant.VERT_COUNT_TOTAL);
        else                   file.read((char*)quadrant.verts, sizeof(gpu::RenderData_General::Vertex) * quadrant.VERT_COUNT_TOTAL);
    }}
}

template<class QUADRANT_T, auto QUADRANT_COUNT>
void WriteBinaryFile(const QUADRANT_T (&quadrants)[QUADRANT_COUNT][QUADRANT_COUNT])
{
    WriteOrRead<true>(quadrants);
}

template<class QUADRANT_T, auto QUADRANT_COUNT>
void ReadBinaryFile(QUADRANT_T (&quadrants)[QUADRANT_COUNT][QUADRANT_COUNT])
{
    WriteOrRead<false>(quadrants);
}

} //ns