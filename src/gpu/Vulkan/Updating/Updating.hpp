//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Updating/Updating_GUI.hpp"
#include "gpu/Vulkan/Updating/Updating_Instances.hpp"
#include "gpu/Vulkan/Updating/Updating_Terrain.hpp"
#include "gpu/Vulkan/Updating/Updating_Post.hpp"

namespace rpg::gpu::vuk {

inline void Update(Resources& resources)
{
    Update_Terrain   (resources);
    Update_Instances (resources);
    Update_Post      (resources);
    Update_GUI       (resources);
}

}//ns