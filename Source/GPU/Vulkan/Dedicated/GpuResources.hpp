//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Dedicated/Shadow/Resources_Shadow.hpp"
#include "GPU/Vulkan/Dedicated/General/Resources_General.hpp"
#include "GPU/Vulkan/Dedicated/Post/Resources_Post.hpp"
#include "GPU/Vulkan/Dedicated/GUI/Resources_GUI.hpp"

#include "Resources/CpuResources.hpp"

namespace rpg::gpu::vuk {

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