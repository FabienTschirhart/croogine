#pragma once

#include "Constants.h"
#include "Croogine_window.h"
#include "Croogine_Pipeline.h"
#include "Croogine_Device.h"
#include "Croogine_SwapChain.h"
#include "Croogine_Entity.h"

#include <memory>
#include <vector>

namespace Croogine {

    class CroogineApp {
    public:

        CroogineApp();
        ~CroogineApp();

        CroogineApp(const CroogineApp&) = delete;
        CroogineApp& operator=(const CroogineApp&) = delete;

        void run();

    private:

        void loadEntities();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void freeCommandBuffers();
        void drawFrame();
        void recreateSwapChain();
        void recordCommandBuffer(int imageIndex);
        void renderEntities(VkCommandBuffer commandBuffer);

        CroogineWindow croogineWindow{ WIDTH, HEIGHT, ENGINE_NAME + ENGINE_VERSION};
        CroogineDevice croogineDevice{ croogineWindow };

        std::unique_ptr<CroogineSwapChain> croogineSwapChain;
        std::unique_ptr<CrooginePipeline> crooginePipeline;
        std::vector<VkCommandBuffer> commandBuffers;
        std::vector<CroogineEntity> entities;

        VkPipelineLayout pipelineLayout;

    };
}