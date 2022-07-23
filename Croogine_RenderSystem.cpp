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
        glm::mat4 modelMatrix{ 1.f };
        glm::mat4 normalMatrix{ 1.f };
    };

    CroogineRenderSystem::CroogineRenderSystem(CroogineDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout) : croogineDevice{ device } {
        createPipelineLayout(globalSetLayout);
        createPipeline(renderPass);
    }

    CroogineRenderSystem::~CroogineRenderSystem() {
        vkDestroyPipelineLayout(croogineDevice.getDevice(), pipelineLayout, nullptr);
    }

   
    void CroogineRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {

        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
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


    void CroogineRenderSystem::renderEntities(Frame &frame, std::vector<CroogineEntity>& entities) {
        crooginePipeline->bind(frame.commandBuffer);

        vkCmdBindDescriptorSets(
            frame.commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            pipelineLayout,
            0, 
            1,
            &frame.globalDescriptorSet,
            0, 
            nullptr);

        for (auto& entity : entities) {

            SimplePushConstantData push{};
            push.modelMatrix = entity.transform.mat4();
            push.normalMatrix = entity.transform.normalMatrix();

            vkCmdPushConstants(
                frame.commandBuffer,
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstantData),
                &push);

            entity.model->bind(frame.commandBuffer);
            entity.model->draw(frame.commandBuffer);
        }
    }

}