#include "Croogine_App.h"

#include <array>
#include <cassert>
#include <stdexcept>

namespace Croogine {

    CroogineApp::CroogineApp() {
        loadModels();
        createPipelineLayout();
        recreateSwapChain();
        createCommandBuffers();
    }

    CroogineApp::~CroogineApp() {
        vkDestroyPipelineLayout(croogineDevice.getDevice(), pipelineLayout, nullptr);
    }

	void CroogineApp::run() {

        while (!croogineWindow.shouldClose()) //Check the close flag of the application window; if there is a click on the close button, it leaves the while()
        {
            glfwPollEvents(); //check all events (click, resize, close, move, etc.) and set flags accordingly
            drawFrame();
        }

        vkDeviceWaitIdle(croogineDevice.getDevice());
	}

    void CroogineApp::loadModels() {
        std::vector<CroogineModel::Vertex> vertices{
            {{0.0f, 0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}}
        };

        croogineModel = std::make_unique<CroogineModel>(croogineDevice, vertices);
    }
        

    void CroogineApp::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        if (vkCreatePipelineLayout(croogineDevice.getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout !");
        }
    }

    void CroogineApp::createPipeline(){

        assert(croogineSwapChain != nullptr && "Cannot create pipeline before swap chain");
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        CrooginePipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = croogineSwapChain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        crooginePipeline = std::make_unique<CrooginePipeline>(
            croogineDevice,
            vertexshader_file,
            fragmentshader_file,
            pipelineConfig
            );
    }

    void CroogineApp::recreateSwapChain()
    {
        auto extent = croogineWindow.getExtent();

        while (extent.width == 0 || extent.height == 0) {
            extent = croogineWindow.getExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(croogineDevice.getDevice());


        if (croogineSwapChain == nullptr) {
            croogineSwapChain = std::make_unique<CroogineSwapChain>(croogineDevice, extent);
        } else {
            croogineSwapChain = std::make_unique<CroogineSwapChain>(croogineDevice, extent, std::move(croogineSwapChain));
            if (croogineSwapChain->imageCount() != commandBuffers.size())
            {
                freeCommandBuffers();
                createCommandBuffers();
            }
        }

        createPipeline();
    }

    void CroogineApp::createCommandBuffers(){

        commandBuffers.resize(croogineSwapChain->imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = croogineDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(croogineDevice.getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) 
            throw std::runtime_error("failed to allocate command buffers !");
    }

    void CroogineApp::freeCommandBuffers()
    {
        vkFreeCommandBuffers(
            croogineDevice.getDevice(),
            croogineDevice.getCommandPool(),
            static_cast<uint32_t>(commandBuffers.size()),
            commandBuffers.data());
        commandBuffers.clear();
    }

    void CroogineApp::recordCommandBuffer(int imageIndex) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) 
            throw std::runtime_error("failed to begin recording command buffer !");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = croogineSwapChain->getRenderPass();
        renderPassInfo.framebuffer = croogineSwapChain->getFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = croogineSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
        clearValues[1].depthStencil = { 1.0f, 0 };
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(croogineSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(croogineSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{ {0, 0}, croogineSwapChain->getSwapChainExtent() };
        vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

        crooginePipeline->bind(commandBuffers[imageIndex]);
        croogineModel->bind(commandBuffers[imageIndex]);
        croogineModel->draw(commandBuffers[imageIndex]);

        vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

    void CroogineApp::drawFrame(){

        uint32_t imageIndex;
        auto result = croogineSwapChain->acquireNextImage(&imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) 
            throw std::runtime_error("failed to acquire swap chain image !");
        

        recordCommandBuffer(imageIndex);
        result = croogineSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || croogineWindow.wasWindowResized()) {
            croogineWindow.resetWindowResizedFlag();
            recreateSwapChain();
            return;
        }else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image !");
        }
    }


}