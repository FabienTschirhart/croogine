#include "Croogine_App.h"


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <cassert>
#include <stdexcept>

namespace Croogine {

    struct SimplePushConstantData {
        glm::mat2 transform{ 1.f };
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };

    CroogineApp::CroogineApp() {
        loadEntities();
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

    void CroogineApp::loadEntities() {
        std::vector<CroogineModel::Vertex> vertices{
            {{0.0f, 0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}}
        };

        auto croogineModel = std::make_shared<CroogineModel>(croogineDevice, vertices);

        auto mesh = CroogineEntity::createEntity();
        mesh.model = croogineModel;
        mesh.color = { .1f, .8f, .1f };
        mesh.transform2D.translation.x = .2f;
        mesh.transform2D.scale = { 1.f, 1.f };
        mesh.transform2D.rotation = -.25f * glm::two_pi<float>();

        entities.push_back(std::move(mesh));
    }
        

    void CroogineApp::createPipelineLayout() {

        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);


        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
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
        clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f }; //background color
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

        renderEntities(commandBuffers[imageIndex]);

        vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

    void CroogineApp::renderEntities(VkCommandBuffer commandBuffer) {
        crooginePipeline->bind(commandBuffer);

        for (auto& entity : entities) {

            entity.transform2D.rotation = glm::mod(entity.transform2D.rotation + 0.001f, glm::two_pi<float>());

            SimplePushConstantData push{};
            push.color = entity.color;
            push.transform = entity.transform2D.mat2();

            vkCmdPushConstants(
                commandBuffer, 
                pipelineLayout, 
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 
                0, 
                sizeof(SimplePushConstantData), 
                &push);

            entity.model->bind(commandBuffer);
            entity.model->draw(commandBuffer);
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