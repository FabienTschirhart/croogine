#pragma once

#include "Croogine_window.h"
#include "Constants.h"
#include "Croogine_Pipeline.h"

namespace Croogine {

    class CroogineApp {
    public:
        void run();

    private:
        CroogineWindow CroogineWindow{ WIDTH, HEIGHT, ENGINE_NAME + ENGINE_VERSION};
        CrooginePipeline CrooginePipeline{ vertexshader_file, fragmentshader_file };
    };

}