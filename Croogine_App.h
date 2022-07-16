#pragma once

#include "Constants.h"
#include "Croogine_window.h"
#include "Croogine_Device.h"
#include "Croogine_Entity.h"
#include "Croogine_Renderer.h"

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

        CroogineWindow croogineWindow{ WIDTH, HEIGHT, ENGINE_NAME + ENGINE_VERSION};
        CroogineDevice croogineDevice{ croogineWindow };
        CroogineRenderer croogineRenderer{ croogineWindow, croogineDevice };

        std::vector<CroogineEntity> entities;
    };
}