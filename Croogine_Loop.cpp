#include "Croogine_Core.h"

void Croogine::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        renderPipeline();
    }

    vkDeviceWaitIdle(device);
}