#include "mini/Window/win_Window.hpp"
#include "mini/Window/win_WindowCallback.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Debug/Console.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Memory/Allocator.hpp"

#include "rpg/Scene/Scene.hpp"
#include "rpg/DeltaTime.hpp"

#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Resources.hpp"

#include <unordered_map>

using namespace mini;



inline VkSubmitInfo SubmitInfo(VkSemaphore& aquired, VkSemaphore& rendered, VkCommandBuffer& cmdBuffer)
{
    static const VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    return {
        .sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext                  = nullptr,
        .waitSemaphoreCount     = 1,
        .pWaitSemaphores        = &aquired,
        .pWaitDstStageMask      = &waitStages,
        .commandBufferCount     = 1,
        .pCommandBuffers        = &cmdBuffer,
        .signalSemaphoreCount   = 1,
        .pSignalSemaphores      = &rendered,
    };
}


inline VkPresentInfoKHR PresentInfo(VkSemaphore& rendered, VkSwapchainKHR& swapchain, uint32_t& imgIndex)
{
    return {
        .sType                  = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext                  = nullptr,
        .waitSemaphoreCount     = 1,
        .pWaitSemaphores        = &rendered,
        .swapchainCount         = 1,
        .pSwapchains            = &swapchain,
        .pImageIndices          = &imgIndex,
        .pResults               = nullptr
    };
}


int WINAPI wWinMain(
    _In_        HINSTANCE hInstance,
    _In_opt_    HINSTANCE hPrevInstance,
    _In_        PWSTR pCmdLine,
    _In_        int nCmdShow)
{
    //? META
    dbg::Console console {};
    wnd::Window  window  { hInstance, 800, 600 };
    mem::GlobalAllocate();

    //? RENDERER
    auto pContext   = mem::ClaimBlock<vk::Context>(vk::WindowHandle{window.hInstance, window.hWnd});
    auto pResources = mem::ClaimBlock<vk::Resources>(pContext.Get());

    //? SCENES
    auto pSceneStack = mem::ClaimBlock<box::Array<rpg::scene::Scene, 4>>();
    pSceneStack->SetCompleteArray();
    auto& currentScene = (*pSceneStack)[0];





    auto& device    = pContext->device;
    auto& swapchain = pContext->swapchain;
    auto& sync      = pResources->default_sync;
    auto& cmds      = pResources->default_commands;
    auto& maxImgs   = pContext->swapImages.count;


    for(auto i = 0; i < 3; ++i)
    {
    auto beginInfo = vk::CreateCmdBeginInfo();
    VK_CHECK(vkBeginCommandBuffer(pResources->default_commands.cmdBuffers[i], &beginInfo));

    const VkClearValue clears [1] {
        VkClearValue {.color = { 0.1f, 0.1f, 0.1f, 1.0f }},
    };

    const VkRenderPassBeginInfo renderPassInfo {
        .sType          = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .pNext          = nullptr,
        .renderPass     = pResources->default_renderPass.renderPass,
        .framebuffer    = pResources->default_renderPass.framebuffers[i],
        .renderArea     = {
            .offset     = VkOffset2D {0, 0},
            .extent     = pContext->surfaceCapabilities.currentExtent
        },
        .clearValueCount= ARRAY_COUNT(clears),
        .pClearValues   = clears
    };

    vkCmdBeginRenderPass(pResources->default_commands.cmdBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(pResources->default_commands.cmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pResources->default_pipeline.pipeline);
    vkCmdDraw(pResources->default_commands.cmdBuffers[i], 3, 1, 0, 0);
    vkCmdEndRenderPass(pResources->default_commands.cmdBuffers[i]);

    VK_CHECK(vkEndCommandBuffer(pResources->default_commands.cmdBuffers[i]));
    }


    //? PROGRAM LOOP
    rpg::dt::StartClock();
    while (!app::CheckEvent(EventType::Window_Close) && !app::IsPressed(EventType::Keyboard_Escape))
    {
        wnd::PollEvents();
        if (const auto fps = rpg::dt::CalcDeltaTimeFPS(); fps > 0)
            LOG("fps", fps);

        currentScene.Update(rpg::dt::seconds);
        currentScene.Render(rpg::dt::seconds); 


        //? rendering (tmp)

        static std::unordered_map<uint32_t, uint32_t> map {};
        static std::unordered_map<uint32_t, uint32_t> mapNew {};
        static std::unordered_map<uint32_t, uint32_t> mapAfter {};

        while(map.size() < maxImgs - 1)
        {
            auto freeSemaphore = sync.semaphores.Get();
            uint32_t imageIndex = 0;
            const auto acquireRes = vkAcquireNextImageKHR(
                device, 
                swapchain, 
                0, 
                sync.semaphores[freeSemaphore], 
                VK_NULL_HANDLE, 
                &imageIndex
            );

            if (acquireRes == VK_SUCCESS)
            {
                if (mapAfter.contains(imageIndex))
                {
                    sync.semaphores.Set<false>(mapAfter.at(imageIndex));
                    mapAfter.erase(imageIndex);
                }

                sync.semaphores.Set<true>(freeSemaphore);
                LOG(imageIndex, freeSemaphore);
                map.emplace(imageIndex, freeSemaphore);
            }
        }

        mapNew.clear();
        for(const auto& [acq, sema] : map)
        {
            const auto fenceRes = vkWaitForFences(device, 1, &sync.fences[acq], VK_FALSE, 0);
            if (fenceRes != VK_SUCCESS) { mapNew.emplace(acq, sema); continue; }
            vkResetFences(device, 1, &sync.fences[acq]);

            const auto submitInfo = SubmitInfo(sync.semaphores[sema], sync.semaphores2[sema], cmds.cmdBuffers[acq]);
            VK_CHECK(vkQueueSubmit(pContext->queue, 1, &submitInfo, sync.fences[acq]));

            auto tmpAcq = acq;
            const auto presentInfo = PresentInfo(sync.semaphores2[sema], swapchain, tmpAcq);
            VK_CHECK(vkQueuePresentKHR(pContext->queue, &presentInfo));
        }

        for(auto& i : map)
            mapAfter.emplace(i);
        map.clear();
        for(auto& i : mapNew)
            map.emplace(i);
        


        /*
        static uint32_t currentFrame = 0;

        vkWaitForFences(device, 1, &sync.fences[currentFrame], VK_FALSE, UINT64_MAX);



        uint32_t imageIndex = 0;
        const auto acquireRes = vkAcquireNextImageKHR(
            device, 
            swapchain, 
            UINT64_MAX, 
            sync.imageAquired[currentFrame], 
            VK_NULL_HANDLE, 
            &imageIndex
        );


        //acquire all
        //map semaphores to imgIndx

        //for all acquired
        //get semaphore via mapping to imgIndx


        if (sync.inFlight[imageIndex] != VK_NULL_HANDLE) {
            vkWaitForFences(device, 1, &sync.inFlight[imageIndex], VK_FALSE, UINT64_MAX);
        }
        sync.inFlight[imageIndex] = sync.fences[currentFrame];

        VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        const auto submitInfo = SubmitInfo(sync.imageAquired[currentFrame], sync.renderDone[currentFrame], cmds.cmdBuffers[imageIndex]);

        vkResetFences(device, 1, &sync.fences[currentFrame]);
        VK_CHECK(vkQueueSubmit(pContext->queue, 1, &submitInfo, pResources->default_sync.fences[currentFrame]));

        const auto presentInfo = PresentInfo(sync.renderDone[currentFrame], swapchain, imageIndex);
        VK_CHECK(vkQueuePresentKHR(pContext->queue, &presentInfo));

        currentFrame = (currentFrame + 1) % (maxImgs - 1);
        */
    }
    
    VK_CHECK(vkDeviceWaitIdle(pContext->device));
    mem::GlobalFree();

}//main end