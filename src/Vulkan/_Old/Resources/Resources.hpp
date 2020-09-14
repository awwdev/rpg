//https://github.com/awwdev

#pragma once

#include "Vulkan/Meta/Context.hpp"
#include "Vulkan/Meta/Commands.hpp"
#include "Vulkan/Meta/Synchronization.hpp"
#include "Vulkan/Objects/PushConstants.hpp" //TODO: move out

#include "Vulkan/_Old/Resources/Resources_Shadow.hpp"
#include "Vulkan/_Old/Resources/Resources_Default.hpp"
#include "Vulkan/_Old/Resources/Resources_Terrain.hpp"
#include "Vulkan/_Old/Resources/Resources_Sky.hpp"
#include "Vulkan/_Old/Resources/Resources_Post.hpp"
#include "Vulkan/_Old/Resources/Resources_UI.hpp"
#include "Vulkan/_Old/Resources/Resources_Test.hpp"

namespace rpg::vuk {

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