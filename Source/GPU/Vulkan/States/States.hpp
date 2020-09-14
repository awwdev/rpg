//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/States/State_Shadow.hpp"
#include "GPU/Vulkan/States/State_General.hpp"
#include "GPU/Vulkan/States/State_Post.hpp"
#include "GPU/Vulkan/States/State_GUI.hpp"

#include "Resources/CpuResources.hpp"

namespace rpg::gpu::vuk {

struct States
{
    State_Shadow  shadow;
    State_General general;
    State_Post    post;
    State_GUI     gui;

    void Create(res::HostResources& hostRes, VkCommandPool cmdPool)
    {
        shadow.Create(cmdPool);
        general.Create(cmdPool);
        post.Create(cmdPool);
        gui.Create(cmdPool);
    }

    void Update()
    {
        shadow.Update();
        general.Update();
        post.Update();
        gui.Update();
    }

    void Record()
    {
        shadow.Record();
        general.Record();
        post.Record();
        gui.Record();
    }
};

} //ns