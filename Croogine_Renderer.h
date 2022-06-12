#pragma once

#include "Croogine_window.h"
#include "Croogine_Device.h"
#include "Croogine_SwapChain.h"

#include <cassert>
#include <memory>
#include <vector>

namespace Croogine {

    class CroogineRenderer {
    public:

        CroogineRenderer(CroogineWindow &window, CroogineDevice &device);
        ~CroogineRenderer();

        CroogineRenderer(const CroogineRenderer&) = delete;
        CroogineRenderer& operator=(const CroogineRenderer&) = delete;

        VkRenderPass getSwapChainRenderPass() const { return croogineSwapChain->getRenderPass(); }
        float getAspectRatio() const { return croogineSwapChain->extentAspectRatio(); }
        bool isFrameInProgress() const { return isFrameStarted; }

        VkCommandBuffer getCurrentCommandBuffer() const { 
            assert(isFrameStarted && "Cannot retrieve command buffer : Frame rendering is not in progress !");
            return commandBuffers[currentFrameIndex]; 
        }

        int getFrameIndex() const {
            assert(isFrameStarted && "Cannot get frame index : no frame is being rendered !");
            return currentFrameIndex;
        }

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSCRenderPass(VkCommandBuffer commandBuffer);
        void endSCRenderPass(VkCommandBuffer commandBuffer);

    private:

        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();


        CroogineWindow& croogineWindow;
        CroogineDevice& croogineDevice;

        std::unique_ptr<CroogineSwapChain> croogineSwapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex{ 0 };
        int currentFrameIndex;
        bool isFrameStarted{ false };
    };
}