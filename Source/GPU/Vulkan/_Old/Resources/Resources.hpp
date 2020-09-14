//https://github.com/awwdev

#pragma once

#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Meta/Commands.hpp"
#include "GPU/Vulkan/Meta/Synchronization.hpp"
#include "GPU/Vulkan/Objects/PushConstants.hpp" //TODO: move out

#include "GPU/Vulkan/_Old/Resources/Resources_Shadow.hpp"
#include "GPU/Vulkan/_Old/Resources/Resources_Default.hpp"
#include "GPU/Vulkan/_Old/Resources/Resources_Terrain.hpp"
#include "GPU/Vulkan/_Old/Resources/Resources_Sky.hpp"
#include "GPU/Vulkan/_Old/Resources/Resources_Post.hpp"
#include "GPU/Vulkan/_Old/Resources/Resources_UI.hpp"
#include "GPU/Vulkan/_Old/Resources/Resources_Test.hpp"

namespace rpg::gpu::vuk {

struct VkResources
{
    //Common_PushConstants  common_pushConsts;
    //Common_PushConstants2 common_pushConsts2;
    //Resources_Shadow  shadow;
    //Resources_Sky     sky;
    //Resources_Terrain terrain; 
    //Resources_Common  common;

    Resources_Test    test;
    Resources_Post    post;
    Resources_UI      ui;


    void Create(res::HostResources& hostRes, VkCommandPool cmdPool)
    {
        //shadow.Create(hostRes, cmdPool);
        //common.Create(hostRes, cmdPool, shadow);
        //terrain.Create(hostRes, cmdPool, shadow, common);
        //sky.Create(cmdPool, common);
        test.Create(cmdPool);
        post.Create(test, cmdPool);
        ui.Create(hostRes, cmdPool);
    }

    void RecreateSwapchain(VkCommandPool cmdPool)
    {
        //shadow.Recreate(cmdPool);
        //common.Recreate(cmdPool, shadow);
        //terrain.Recreate(cmdPool, shadow, common);    
        //sky.Recreate(cmdPool, common);    
        test.Recreate(cmdPool);    
        post.Recreate(test);       
        ui.Recreate();
    } 
};

} //ns