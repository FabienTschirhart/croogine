#include "Croogine_Renderer.h"

#include <array>
#include <cassert>
#include <stdexcept>

namespace Croogine {

    CroogineRenderer::CroogineRenderer(CroogineWindow& window, CroogineDevice& device) : croogineWindow{ window }, croogineDevice{ device } {
        recreateSwapChain();
        createCommandBuffers();
    }

    CroogineRenderer::~CroogineRenderer() {
        freeCommandBuffers();
    }

    void CroogineRenderer::recreateSwapChain()
    {
        auto extent = croogineWindow.getExtent();

        while (extent.width == 0 || extent.height == 0) {
            extent = croogineWindow.getExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(croogineDevice.getDevice());


        if (croogineSwapChain == nullptr) {
            croogineSwapChain = std::make_unique<CroogineSwapChain>(croogineDevice, extent);
        }
        else {
            std::shared_ptr<CroogineSwapChain> oldSwapChain = std::move(croogineSwapChain);
            croogineSwapChain = std::make_unique<CroogineSwapChain>(croogineDevice, extent, std::move(croogineSwapChain));
            if (!oldSwapChain->compareSwapFormat(*croogineSwapChain.get()))
                throw std::runtime_error("Swap chain image or depth format has changed !");
        }
    }

    void CroogineRenderer::createCommandBuffers() {

        commandBuffers.resize(CroogineSwapChain::MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = croogineDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(croogineDevice.getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
            throw std::runtime_error("failed to allocate command buffers !");
    }

    void CroogineRenderer::freeCommandBuffers()
    {
        vkFreeCommandBuffers(
            croogineDevice.getDevice(),
            croogineDevice.getCommandPool(),
            static_cast<uint32_t>(commandBuffers.size()),
            commandBuffers.data());
        commandBuffers.clear();
    }

    VkCommandBuffer CroogineRenderer::beginFrame() {
        assert(!isFrameStarted && "Cannot begin a new frame : a frame is already being rendered !");

        auto result = croogineSwapChain->acquireNextImage(&currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return nullptr;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            throw std::runtime_error("failed to acquire swap chain image !");

        isFrameStarted = true;

        auto commandBuffer = getCurrentCommandBuffer();

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
            throw std::runtime_error("failed to begin recording command buffer !");

        return commandBuffer;
    }

    void CroogineRenderer::endFrame() {
        assert(isFrameStarted && "Cannot end the frame : there is not frame being rendered");

        auto commandBuffer = getCurrentCommandBuffer();

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) 
            throw std::runtime_error("failed to record command buffer!");
        

        auto result = croogineSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || croogineWindow.wasWindowResized()) 
        {
            croogineWindow.resetWindowResizedFlag();
            recreateSwapChain();
        }
        else if (result != VK_SUCCESS) 
            throw std::runtime_error("failed to present swap chain image !");
        isFrameStarted = false;
        currentFrameIndex = (currentFrameIndex + 1) % CroogineSwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void CroogineRenderer::beginSCRenderPass(VkCommandBuffer commandBuffer) {
        assert(isFrameStarted && "Cannot begin swap chain render pass : there is not frame being rendered");
        assert(commandBuffer == getCurrentCommandBuffer() && "Cannot begin render pass on this command buffer as it is not the same frame");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = croogineSwapChain->getRenderPass();
        renderPassInfo.framebuffer = croogineSwapChain->getFrameBuffer(currentImageIndex);

        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = croogineSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f }; //background color
        clearValues[1].depthStencil = { 1.0f, 0 };
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(croogineSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(croogineSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{ {0, 0}, croogineSwapChain->getSwapChainExtent() };
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    }

    void CroogineRenderer::endSCRenderPass(VkCommandBuffer commandBuffer) {
        assert(isFrameStarted && "Cannot end swap chain render pass : there is not frame being rendered");
        assert(commandBuffer == getCurrentCommandBuffer() && "Cannot end render pass on this command buffer as it is not the same frame");

        vkCmdEndRenderPass(commandBuffer);
    }

}