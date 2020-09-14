//https://github.com/awwdev

#pragma once
#include "Vulkan/Dedicated/Shadow/Resources_Shadow.hpp"
#include "Vulkan/Dedicated/General/Resources_General.hpp"
#include "Vulkan/Dedicated/Post/Resources_Post.hpp"
#include "Vulkan/Dedicated/GUI/Resources_GUI.hpp"

#include "Resources/CpuResources.hpp"

namespace rpg::vuk {

struct GpuResources
{
    Resources_Shadow  shadow;
    Resources_General general;
    Resources_Post    post;
    Resources_GUI     gui;

    void Create(res::HostResources& hostRes, VkCommandPool cmdPool)
    {
        shadow.Create(cmdPool);
        general.Create(cmdPool);
        post.Create(cmdPool);
        gui.Create(cmdPool);
    }
};

} //ns