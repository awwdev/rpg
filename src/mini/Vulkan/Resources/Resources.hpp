//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Commands.hpp"
#include "mini/Vulkan/Synchronization.hpp"
#include "mini/Vulkan/Objects/PushConstants.hpp" //TODO: move out

#include "mini/Vulkan/Resources/Resources_Shadow.hpp"
#include "mini/Vulkan/Resources/Resources_Default.hpp"
#include "mini/Vulkan/Resources/Resources_Terrain.hpp"
#include "mini/Vulkan/Resources/Resources_Sky.hpp"
#include "mini/Vulkan/Resources/Resources_UI.hpp"

namespace mini::vk {

struct VkResources
{
    Common_PushConstants  common_pushConsts;
    Common_PushConstants2 common_pushConsts2;

    Resources_Shadow  shadow;
    Resources_Sky     sky;
    Resources_Terrain terrain;
    Resources_Default default;
    Resources_UI      ui;

    void Create(res::HostResources& hostRes, VkCommandPool cmdPool)
    {
        shadow.Create(hostRes, cmdPool);
        default.Create(hostRes, cmdPool, shadow);
        terrain.Create(hostRes, cmdPool, shadow, default);
        sky.Create(cmdPool, default);
        ui.Create(hostRes, cmdPool);
    }

    void RecreateSwapchain(VkCommandPool cmdPool)
    {
        shadow.Recreate(cmdPool);
        default.Recreate(cmdPool, shadow);
        terrain.Recreate(cmdPool, shadow, default);    
        sky.Recreate(cmdPool, default);        
        ui.Recreate();
    } 
};

} //ns