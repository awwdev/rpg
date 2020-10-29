//https://github.com/awwdev

#pragma once
#include "res/Resources_Terrain.hpp"
#include "com/box/String.hpp"
#include "dbg/Assert.hpp"

#include <fstream>

namespace rpg::res {

inline void SaveTerrain(res::Resources_Terrain const& resTerrain)
{
    auto const& terrain = resTerrain.terrain;
    FOR_C_ARRAY(terrain.quadrants, i)
    {
        //file
        com::String<40> path { "tmp/terrain_quadrant_mesh_" };
        path.AppendArithemtic(i);
        auto file = std::ofstream(path.Data(), std::ios::binary);
        dbg::Assert(file.is_open(), "cannot open file");
        //data
        auto const& quadrant = terrain.quadrants[i];
        file.write(reinterpret_cast<char const*>(&quadrant.mesh), sizeof(quadrant.mesh));
    }
}

inline void LoadTerrain(res::Resources_Terrain& resTerrain)
{
    auto& terrain = resTerrain.terrain;
    FOR_C_ARRAY(terrain.quadrants, i)
    {
        //file
        com::String<40> path { "tmp/terrain_quadrant_mesh_" };
        path.AppendArithemtic(i);
        auto file = std::ifstream(path.Data(), std::ios::binary);
        dbg::Assert(file.is_open(), "cannot open file");
        //data
        auto& quadrant = terrain.quadrants[i];
        file.read(reinterpret_cast<char*>(&quadrant.mesh), sizeof(quadrant.mesh));
    }
}

}//ns