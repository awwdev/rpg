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


    //? PROGRAM LOOP
    rpg::dt::StartClock();
    while (!app::CheckEvent(EventType::Window_Close) && !app::IsPressed(EventType::Keyboard_Escape))
    {
        wnd::PollEvents();
        rpg::dt::CalcDeltaTime();

        currentScene.Update(rpg::dt::seconds);
        currentScene.Render(rpg::dt::seconds); 
        //TODO: scene.render or render(scene) how is this done


        //? rendering (tmp)
    
        uint32_t imageIndex = 0;
        static uint32_t currentFrame = 0;

        VK_CHECK(vkWaitForFences(pContext->device, 1, &pResources->default_semaphores.fences[currentFrame], VK_TRUE, UINT64_MAX));

        const auto res = vkAcquireNextImageKHR(
            pContext->device, 
            pContext->swapchain, 
            UINT64_MAX,
            pResources->default_semaphores.imageAquired[currentFrame],
            VK_NULL_HANDLE, 
            &imageIndex
        ); //! check result

        VK_CHECK(vkResetFences(pContext->device, 1, &pResources->default_semaphores.fences[currentFrame]));
        
        //if (res == VK_NOT_READY) continue;
    
        LOG("frame", currentFrame);
        LOG("img index", imageIndex);


        auto beginInfo = vk::CreateCmdBeginInfo();
        VK_CHECK(vkBeginCommandBuffer(pResources->default_commands.cmdBuffers[imageIndex], &beginInfo));

        const VkClearValue clears [1] {
            VkClearValue {.color = { 0.1f, 0.1f, 0.1f, 1.0f }},
        };

        const VkRenderPassBeginInfo renderPassInfo {
            .sType          = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext          = nullptr,
            .renderPass     = pResources->default_renderPass.renderPass,
            .framebuffer    = pResources->default_renderPass.framebuffers[imageIndex],
            .renderArea     = {
                .offset     = VkOffset2D {0, 0},
                .extent     = pContext->surfaceCapabilities.currentExtent
            },
            .clearValueCount= ARRAY_COUNT(clears),
            .pClearValues   = clears
        };

        vkCmdBeginRenderPass(pResources->default_commands.cmdBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(pResources->default_commands.cmdBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, pResources->default_pipeline.pipeline);
        vkCmdDraw(pResources->default_commands.cmdBuffers[imageIndex], 3, 1, 0, 0);
        vkCmdEndRenderPass(pResources->default_commands.cmdBuffers[imageIndex]);

        VK_CHECK(vkEndCommandBuffer(pResources->default_commands.cmdBuffers[imageIndex]));
        

        VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        const VkSubmitInfo submitInfo {
            .sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext                  = nullptr,
            .waitSemaphoreCount     = 1,
            .pWaitSemaphores        = &pResources->default_semaphores.imageAquired[currentFrame],
            .pWaitDstStageMask      = &waitStages,
            .commandBufferCount     = 1,
            .pCommandBuffers        = &pResources->default_commands.cmdBuffers[imageIndex],
            .signalSemaphoreCount   = 1,
            .pSignalSemaphores      = &pResources->default_semaphores.renderDone[currentFrame],
        };

        VK_CHECK(vkQueueSubmit(pContext->queue, 1, &submitInfo, pResources->default_semaphores.fences[currentFrame]));




        const VkPresentInfoKHR presentInfo {
            .sType                  = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext                  = nullptr,
            .waitSemaphoreCount     = 1,
            .pWaitSemaphores        = &pResources->default_semaphores.renderDone[currentFrame],
            .swapchainCount         = 1,
            .pSwapchains            = &pContext->swapchain,
            .pImageIndices          = &imageIndex,
            .pResults               = nullptr
        };
        VK_CHECK(vkQueuePresentKHR(pContext->queue, &presentInfo));

        //VK_CHECK(vkDeviceWaitIdle(pContext->device));

        currentFrame = (currentFrame + 1) % pContext->swapImages.count;
    }
    
    mem::GlobalFree();

}//main end