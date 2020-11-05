//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Recording/Recording_GUI.hpp"
#include "gpu/Vulkan/Recording/Recording_Instances.hpp"
#include "gpu/Vulkan/Recording/Recording_Terrain.hpp"
#include "gpu/Vulkan/Recording/Recording_Post.hpp"

namespace rpg::gpu::vuk {

inline void Record()
{
    Record_TerrainShadow();
    Record_InstancesShadow();
    Record_Terrain();
    Record_Instances();
    Record_Post();
    Record_GUI();
}

}//ns