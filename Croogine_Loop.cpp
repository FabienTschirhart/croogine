#include "Croogine_Core.h"

void Croogine::mainLoop() {

    while (!glfwWindowShouldClose(window)) //Check the close flag of the application window; if there is a click on the close button, it leaves the while()
    {
        glfwPollEvents(); //check all events (click, resize, close, move, etc.) and set flags accordingly
        renderPipeline(); 
    }

    vkDeviceWaitIdle(device);
}