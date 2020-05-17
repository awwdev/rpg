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

using namespace mini;

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

        auto& device    = pContext->device;
        auto& swapchain = pContext->swapchain;
        auto& sync      = pResources->default_sync;
        auto& maxImgs   = pContext->swapImages.count;

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

        if (sync.inFlight[imageIndex] != VK_NULL_HANDLE) {
            vkWaitForFences(device, 1, &sync.inFlight[imageIndex], VK_FALSE, UINT64_MAX);
        }
        sync.inFlight[imageIndex] = sync.fences[currentFrame];


        VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        const VkSubmitInfo submitInfo {
            .sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext                  = nullptr,
            .waitSemaphoreCount     = 1,
            .pWaitSemaphores        = &pResources->default_sync.imageAquired[currentFrame],
            .pWaitDstStageMask      = &waitStages,
            .commandBufferCount     = 1,
            .pCommandBuffers        = &pResources->default_commands.cmdBuffers[imageIndex],
            .signalSemaphoreCount   = 1,
            .pSignalSemaphores      = &pResources->default_sync.renderDone[currentFrame],
        };

        vkResetFences(device, 1, &sync.fences[currentFrame]);
        VK_CHECK(vkQueueSubmit(pContext->queue, 1, &submitInfo, pResources->default_sync.fences[currentFrame]));

        const VkPresentInfoKHR presentInfo {
            .sType                  = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext                  = nullptr,
            .waitSemaphoreCount     = 1,
            .pWaitSemaphores        = &pResources->default_sync.renderDone[currentFrame],
            .swapchainCount         = 1,
            .pSwapchains            = &pContext->swapchain,
            .pImageIndices          = &imageIndex,
            .pResults               = nullptr
        };
        VK_CHECK(vkQueuePresentKHR(pContext->queue, &presentInfo));

        currentFrame = (currentFrame + 1) % (maxImgs - 1);

    }
    
    VK_CHECK(vkDeviceWaitIdle(pContext->device));
    mem::GlobalFree();

}//main end