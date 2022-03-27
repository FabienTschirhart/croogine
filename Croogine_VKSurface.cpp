#include "Croogine_Core.h"
#include "Croogine_constants.h"

void Croogine::createSurface() {
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}