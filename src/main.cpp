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




    mini::vk::VkArray<VkSemaphore, 4> semaphores; 
    semaphores.count = pContext->images.count;
    FOR_VK_ARRAY(semaphores, i)
    {
        VkSemaphoreCreateInfo semaInfo { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO , nullptr , 0 };
        vkCreateSemaphore(pContext->device, &semaInfo, nullptr, &semaphores[i]);    
    }

    
    


    //? PROGRAM LOOP
    rpg::dt::StartClock();
    while (!app::CheckEvent(EventType::Window_Close) && !app::IsPressed(EventType::Keyboard_Escape))
    {
        wnd::PollEvents();
        rpg::dt::CalcDeltaTime();

        currentScene.Update(rpg::dt::seconds);
        currentScene.Render(rpg::dt::seconds);


        //? rendering (tmp)
    
        uint32_t imageIndex = 0;
        
    
        const auto res = vkAcquireNextImageKHR(
            pContext->device, 
            pContext->swapchain, 
            UINT64_MAX,
            semaphores[imageIndex],
            VK_NULL_HANDLE, 
            &imageIndex
        );
    
        const VkCommandBufferBeginInfo beginInfo {
            .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext            = nullptr,
            .flags            = 0,
            .pInheritanceInfo = nullptr
        };

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
            .pWaitSemaphores        = &semaphores[imageIndex],
            .pWaitDstStageMask      = &waitStages,
            .commandBufferCount     = 1,
            .pCommandBuffers        = &pResources->default_commands.cmdBuffers[imageIndex],
            .signalSemaphoreCount   = 0, //! fill out I guess
            .pSignalSemaphores      = nullptr,
        };

        VK_CHECK(vkQueueSubmit(pContext->queue, 1, &submitInfo, VK_NULL_HANDLE));

        const VkPresentInfoKHR presentInfo {
            .sType                  = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext                  = nullptr,
            .waitSemaphoreCount     = 0,
            .pWaitSemaphores        = nullptr,
            .swapchainCount         = 1,
            .pSwapchains            = &pContext->swapchain,
            .pImageIndices          = &imageIndex,
            .pResults               = nullptr
        };
        VK_CHECK(vkQueuePresentKHR(pContext->queue, &presentInfo));


        VK_CHECK(vkDeviceWaitIdle(pContext->device));
    }
    
    mem::GlobalFree();

}//main end