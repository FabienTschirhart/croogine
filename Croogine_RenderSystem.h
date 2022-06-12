#pragma once

#include "Croogine_Camera.h"
#include "Croogine_Pipeline.h"
#include "Croogine_Device.h"
#include "Croogine_Entity.h"

#include <memory>
#include <vector>

namespace Croogine {

    class CroogineRenderSystem {
    public:

        CroogineRenderSystem(CroogineDevice& device, VkRenderPass renderPass);
        ~CroogineRenderSystem();

        CroogineRenderSystem(const CroogineRenderSystem&) = delete;
        CroogineRenderSystem& operator=(const CroogineRenderSystem&) = delete;

        void renderEntities(VkCommandBuffer commandBuffer, std::vector<CroogineEntity> &entities, const CroogineCamera &camera);

    private:

        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);

        CroogineDevice& croogineDevice;

        std::unique_ptr<CrooginePipeline> crooginePipeline;
        VkPipelineLayout pipelineLayout;

    };
}