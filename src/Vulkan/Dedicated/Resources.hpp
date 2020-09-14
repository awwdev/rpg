//https://github.com/awwdev

#pragma once
#include "Vulkan/Dedicated/General/_General_Resources.hpp"
#include "Vulkan/Dedicated/Post/_Post_Resources.hpp"
#include "Vulkan/Dedicated/GUI/_GUI_Resources.hpp"

#include "Resources/HostResources.hpp"

namespace rpg::vuk {

struct VukResources
{
    Resources_General general;

    void Create(res::HostResources& hostRes, VkCommandPool cmdPool)
    {
        general.Create(cmdPool);
    }
};

} //ns