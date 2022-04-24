#pragma once

#include "Constants.h"
#include "Croogine_window.h"
#include "Croogine_Pipeline.h"
#include "Croogine_Device.h"
#include "Croogine_SwapChain.h"

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

        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();

        CroogineWindow croogineWindow{ WIDTH, HEIGHT, ENGINE_NAME + ENGINE_VERSION};
        CroogineDevice croogineDevice{ croogineWindow };
        CroogineSwapChain croogineSwapChain{ croogineDevice, croogineWindow.getExtent() };

        std::unique_ptr<CrooginePipeline> crooginePipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;

    };
}