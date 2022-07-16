#pragma once

#include "Croogine_Camera.h"
#include "Croogine_Pipeline.h"
#include "Croogine_Device.h"
#include "Croogine_Entity.h"
#include "Croogine_Frame.h"

#include <memory>
#include <vector>

namespace Croogine {

    class CroogineRenderSystem {
    public:

        CroogineRenderSystem(CroogineDevice& device, VkRenderPass renderPass);
        ~CroogineRenderSystem();

        CroogineRenderSystem(const CroogineRenderSystem&) = delete;
        CroogineRenderSystem& operator=(const CroogineRenderSystem&) = delete;

        void renderEntities(
            Frame &frame, 
            std::vector<CroogineEntity> &entities 
            );

    private:

        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);

        CroogineDevice& croogineDevice;

        std::unique_ptr<CrooginePipeline> crooginePipeline;
        VkPipelineLayout pipelineLayout;

    };
}