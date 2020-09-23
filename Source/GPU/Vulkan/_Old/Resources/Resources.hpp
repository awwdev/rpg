//https://github.com/awwdev

#pragma once

#include "gpu/Vulkan/Meta/Context.hpp"
#include "gpu/Vulkan/Meta/Commands.hpp"
#include "gpu/Vulkan/Meta/Synchronization.hpp"
#include "gpu/Vulkan/_Old/Objects/PushConstants.hpp" //TODO: move out

#include "gpu/Vulkan/_Old/res/Resources_Shadow.hpp"
#include "gpu/Vulkan/_Old/res/Resources_Default.hpp"
#include "gpu/Vulkan/_Old/res/Resources_Terrain.hpp"
#include "gpu/Vulkan/_Old/res/Resources_Sky.hpp"
#include "gpu/Vulkan/_Old/res/Resources_Post.hpp"
#include "gpu/Vulkan/_Old/res/Resources_UI.hpp"
#include "gpu/Vulkan/_Old/res/Resources_Test.hpp"

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


    void Create(res::CpuResources& hostRes, VkCommandPool cmdPool)
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