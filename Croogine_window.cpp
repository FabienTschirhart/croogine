
#include "Croogine_Core.h"
#include "Croogine_constants.h"

const std::string WINDOW_TITLE = ENGINE_NAME + " " + ENGINE_VERSION;

void Croogine::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<Croogine*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}

void Croogine::initWindow() {
    //GLFW (Graphic Library FrameWork) is required to render the result within a window (online rendering).
    //glfwini() will initialize the GLFW library.
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //By default, GLFW will generate an OpenGL context window. As we are using Vulkan instead of OpenGL
    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //We don't want the window to be resizable

    window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_TITLE.c_str(), nullptr, nullptr); //define window size and title (both are declared in Croogine_constants.h). No monitor for fulllscreen.
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

