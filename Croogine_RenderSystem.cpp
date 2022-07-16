#include "Croogine_RenderSystem.h"


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <cassert>
#include <stdexcept>

namespace Croogine {

    struct SimplePushConstantData {
        glm::mat4 transform{ 1.f };
        glm::mat4 normalMatrix{ 1.f };
    };

    CroogineRenderSystem::CroogineRenderSystem(CroogineDevice& device, VkRenderPass renderPass) : croogineDevice{ device } {
        createPipelineLayout();
        createPipeline(renderPass);
    }

    CroogineRenderSystem::~CroogineRenderSystem() {
        vkDestroyPipelineLayout(croogineDevice.getDevice(), pipelineLayout, nullptr);
    }

   
    void CroogineRenderSystem::createPipelineLayout() {

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

    void CroogineRenderSystem::createPipeline(VkRenderPass renderPass) {

        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        CrooginePipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        crooginePipeline = std::make_unique<CrooginePipeline>(
            croogineDevice,
            VERTEX_SHADER_FILE,
            FRAGMENT_SHADER_FILE,
            pipelineConfig
            );
    }


    void CroogineRenderSystem::renderEntities(VkCommandBuffer commandBuffer, std::vector<CroogineEntity>& entities, const CroogineCamera& camera) {
        crooginePipeline->bind(commandBuffer);

        auto projectionView = camera.getProjection() * camera.getView();

        for (auto& entity : entities) {

            SimplePushConstantData push{};
            auto normalMatrix = entity.transform.mat4();
            push.transform = projectionView * normalMatrix;
            push.normalMatrix = entity.transform.normalMatrix();

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