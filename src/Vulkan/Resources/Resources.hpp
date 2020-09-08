//https://github.com/awwdev

#pragma once
#include "Vulkan/Core.hpp"
#include "Vulkan/Context.hpp"
#include "Vulkan/Commands.hpp"
#include "Vulkan/Synchronization.hpp"
#include "Vulkan/Objects/PushConstants.hpp" //TODO: move out

#include "Vulkan/Resources/Resources_Shadow.hpp"
#include "Vulkan/Resources/Resources_Default.hpp"
#include "Vulkan/Resources/Resources_Terrain.hpp"
#include "Vulkan/Resources/Resources_Sky.hpp"
#include "Vulkan/Resources/Resources_UI.hpp"

namespace rpg::vk {

struct VkResources
{
    Common_PushConstants  common_pushConsts;
    Common_PushConstants2 common_pushConsts2;

    Resources_Shadow  shadow;
    Resources_Sky     sky;
    Resources_Terrain terrain;
    Resources_Common  common;
    Resources_UI      ui;

    void Create(res::HostResources& hostRes, VkCommandPool cmdPool)
    {
        shadow.Create(hostRes, cmdPool);
        common.Create(hostRes, cmdPool, shadow);
        terrain.Create(hostRes, cmdPool, shadow, common);
        sky.Create(cmdPool, common);
        ui.Create(hostRes, cmdPool);
    }

    void RecreateSwapchain(VkCommandPool cmdPool)
    {
        shadow.Recreate(cmdPool);
        common.Recreate(cmdPool, shadow);
        terrain.Recreate(cmdPool, shadow, common);    
        sky.Recreate(cmdPool, common);        
        ui.Recreate();
    } 
};

} //ns