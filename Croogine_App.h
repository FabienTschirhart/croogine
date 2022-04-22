#pragma once

#include "Croogine_window.h"
#include "Constants.h"

namespace Croogine {

    class CroogineApp {
    public:
        void run();

    private:
        CroogineWindow CroogineWindow{ WIDTH, HEIGHT, ENGINE_NAME + ENGINE_VERSION};
    };

}