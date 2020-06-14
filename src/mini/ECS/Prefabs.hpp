//https://github.com/awwdev

#pragma once
#include "mini/ECS/ComponentArray.hpp"

namespace mini::ecs
{
    enum PrefabType
    {
        UI_FpsMonitor,
        ENUM_END
    };

    struct Prefabs
    {
        ComponentArrays<PrefabType::ENUM_END> arrays;

        inline void Load(chars_t path)
        {
            
        }
    };    

}//ns