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
        createPipeline();
    }

    CroogineApp::~CroogineApp() {
        vkDestroyPipelineLayout(croogineDevice.getDevice(), pipelineLayout, nullptr);
    }

	void CroogineApp::run() {

        while (!croogineWindow.shouldClose()) //Check the close flag of the application window; if there is a click on the close button, it leaves the while()
        {
            glfwPollEvents(); //check all events (click, resize, close, move, etc.) and set flags accordingly

            if (auto commandBuffer = croogineRenderer.beginFrame()) {
                croogineRenderer.beginSwapChainRenderPass(commandBuffer);
                renderEntities(commandBuffer);
                croogineRenderer.endSwapChainRenderPass(commandBuffer);
                croogineRenderer.endFrame();
            }
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

        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        CrooginePipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = croogineRenderer.getSwapChainRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        crooginePipeline = std::make_unique<CrooginePipeline>(
            croogineDevice,
            vertexshader_file,
            fragmentshader_file,
            pipelineConfig
            );
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

}