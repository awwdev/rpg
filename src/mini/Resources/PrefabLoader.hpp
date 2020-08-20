//https://github.com/awwdev

#pragma once
#include "mini/ECS/PrefabTypes.hpp"
#include "mini/ECS/ComponentArray.hpp"
#include "mini/Debug/Assert.hpp"

#include <fstream>

namespace mini::res {

template<auto MAX_COUNT>
void LoadPrefabs(chars_t path, ecs::ComponentArrays<MAX_COUNT>& componentArrays)
{
    std::ifstream file(path, std::ios::beg);
    dbg::Assert(file.is_open(), "cannot open file");

    constexpr auto LINE_CHARS_MAX = 30;
    char line [LINE_CHARS_MAX];
    while(file.getline(line, LINE_CHARS_MAX))  {

        

    }
}

}//NS