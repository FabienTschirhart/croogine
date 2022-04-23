#pragma once

#include "Constants.h"
#include "Croogine_window.h"
#include "Croogine_Pipeline.h"
#include "Croogine_Device.h"

namespace Croogine {

    class CroogineApp {
    public:
        void run();

    private:
        CroogineWindow croogineWindow{ WIDTH, HEIGHT, ENGINE_NAME + ENGINE_VERSION};
        CroogineDevice croogineDevice{ croogineWindow };
        VkPipelineLayout pipelineLayout;
        CrooginePipeline CrooginePipeline{ croogineDevice, vertexshader_file, fragmentshader_file, CrooginePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT) };
    };

}