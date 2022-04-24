#include "Croogine_App.h"

#include <array>
#include <stdexcept>

namespace Croogine {

    CroogineApp::CroogineApp() {
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    CroogineApp::~CroogineApp() {
        vkDestroyPipelineLayout(croogineDevice.getDevice(), pipelineLayout, nullptr);

    }

	void CroogineApp::run() {

        while (!croogineWindow.shouldClose()) //Check the close flag of the application window; if there is a click on the close button, it leaves the while()
        {
            glfwPollEvents(); //check all events (click, resize, close, move, etc.) and set flags accordingly
            //renderPipeline();
        }
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
        PipelineConfigInfo pipelineConfig{};
        CrooginePipeline::defaultPipelineConfigInfo(pipelineConfig, croogineSwapChain.width(), croogineSwapChain.height());
        pipelineConfig.renderPass = croogineSwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        crooginePipeline = std::make_unique<CrooginePipeline>(
            croogineDevice,
            vertexshader_file,
            fragmentshader_file,
            pipelineConfig
            );
    }

    void CroogineApp::createCommandBuffers()
    {
    }

    void CroogineApp::drawFrame()
    {
    }


}