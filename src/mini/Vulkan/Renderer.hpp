//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Resources.hpp"
#include "mini/Resources/HostResources.hpp"
#include "mini/App/Scene.hpp"
#include "mini/Rendering/RenderGraph.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/Box/String.hpp"
#include "mini/Window/WindowEvents.hpp"
#include "mini/Utils/DeltaTime.hpp"
#include "mini/Vulkan/RenderCommands.hpp"

namespace mini::vk {

struct VkRenderer
{
    Context         context;
    VkResources     resources;
    Commands        commands;
    Synchronization sync;
    uint32_t        currentFrame = 0;
    
    VkRenderer(const vk::WindowHandle& wndHandle, res::HostResources& hostResources)
    {
        context.Create(wndHandle); //there is a global ptr to vk context
        sync.Create();
        commands.Create();
        resources.Create(hostResources, commands.cmdPool);
    }

    void RecreateScwapchain()
    {
        vkDeviceWaitIdle(context.device);
        if (!context.RecreateSwapchain())
            return;

        resources.RecreateSwapchain(commands.cmdPool);

        commands.~Commands();
        commands.Create();
    }

    void UpdateVkResources_GameScene(const app::GameScene& scene, res::HostResources& hostRes, double dt)
    {
        const utils::Mat4f BIAS { 
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.5, 0.5, 0.0, 1.0 
        };

        //resources.common_pushConsts.camera = scene.camera.GetOrthographic() * scene.sun.GetView();
        //resources.common_pushConsts.camera = scene.camera.GetPerspective()  * scene.playerController.GetView(scene.ecs);
        //resources.common_pushConsts.camera = scene.camera.GetPerspective()  * scene.camera.GetView();

        //TODO: this should be handled somewhere else which is active
        if (app::global::inputMode == app::global::PlayMode)
            resources.common_pushConsts.camera = resources.sky.pushConsts.camera = scene.playerController.camera.perspective * scene.playerController.camera.view;
        else
            resources.common_pushConsts.camera = resources.sky.pushConsts.camera = scene.editorController.camera.perspective * scene.editorController.camera.view;
            //resources.common_pushConsts.camera = scene.sun.GetOrthographic() * scene.sun.GetView();

        resources.common_pushConsts.sun    = scene.sun.GetOrthographic() * scene.sun.GetView(); //BIAS * 
        resources.common_pushConsts.sunDir = utils::Normalize(scene.sun.pos * 1);
        resources.ui.pushConsts.wnd_w = wnd::global::window_w;
        resources.ui.pushConsts.wnd_h = wnd::global::window_h;
        resources.sky.pushConsts.topColor = { 0.1f, 0.1f, 1.0f, 1 };
        resources.sky.pushConsts.botColor = { 1.0f, 1.0f, 1.0f, 1 };

        resources.ui.ubo.Clear();
        resources.ui.ubo.Store(scene.renderGraph.ui_ubo);

        resources.default.ubo.Clear();
        resources.default.ubo.Store(scene.renderGraph.default_ubo);

        //update terrain quadrant that is edited
        FOR_ARRAY(hostRes.terrain.editing.dirtyQuadrants, i){
            const auto quadrantIdx = hostRes.terrain.editing.dirtyQuadrants[i];
            resources.terrain.vbo.UpdateGroup(quadrantIdx, hostRes.terrain.GetQuadrant(quadrantIdx).verts);
        }
        
    }

    void Render(const double dt, app::GameScene& scene, res::HostResources& hostRes)
    {
        if (wnd::global::events[wnd::Window_Resize] == wnd::Set){
            RecreateScwapchain();
            return;
        } //checking for size==0 is done previously (will also pause other logic)

        if (vkWaitForFences(context.device, 1, &sync.fences[currentFrame], VK_FALSE, 0) != VK_SUCCESS)
            return;

        uint32_t imageIndex = 0;
        const auto acquireRes = vkAcquireNextImageKHR(
            context.device, 
            context.swapchain, 
            0, 
            sync.imageAcquired[currentFrame], 
            VK_NULL_HANDLE, 
            &imageIndex
        ); 

        switch(acquireRes)
        {
            case VK_SUCCESS: break;
            case VK_ERROR_OUT_OF_DATE_KHR: RecreateScwapchain(); return; //when?!
            default: return;
        }

        if (sync.inFlight[imageIndex] != VK_NULL_HANDLE) {
            vkWaitForFences(context.device, 1, &sync.inFlight[imageIndex], VK_FALSE, UINT64_MAX); //important to wait
        }
        sync.inFlight[imageIndex] = sync.fences[currentFrame];
        VK_CHECK(vkResetFences(context.device, 1, &sync.fences[currentFrame]));

        //!UPDATE GPU RESOURCES AND RECORD COMMANDS----------
        UpdateVkResources_GameScene(scene, hostRes, dt); //TODO: depends on scene
        RecordCommands(resources, commands, imageIndex, dt, scene);
        //!--------------------------------------------------

        const VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        const VkSubmitInfo submitInfo {
            .sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext                  = nullptr,
            .waitSemaphoreCount     = 1,
            .pWaitSemaphores        = &sync.imageAcquired[currentFrame],
            .pWaitDstStageMask      = &waitStages,
            .commandBufferCount     = 1,
            .pCommandBuffers        = &commands.cmdBuffers[imageIndex],
            .signalSemaphoreCount   = 1,
            .pSignalSemaphores      = &sync.imageFinished[currentFrame],
        };
        VK_CHECK(vkQueueSubmit(context.queue, 1, &submitInfo, sync.fences[currentFrame]));

        const VkPresentInfoKHR presentInfo {
            .sType                  = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext                  = nullptr,
            .waitSemaphoreCount     = 1,
            .pWaitSemaphores        = &sync.imageFinished[currentFrame],
            .swapchainCount         = 1,
            .pSwapchains            = &context.swapchain,
            .pImageIndices          = &imageIndex,
            .pResults               = nullptr
        };
        VK_CHECK(vkQueuePresentKHR(context.queue, &presentInfo));

        currentFrame = (currentFrame + 1) % (context.swapImages.count - 1);
    }
};

}//ns