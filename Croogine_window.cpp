
//#include "Croogine_Core.h"
#include "Croogine_window.h"
#include "Constants.h"

#include <stdexcept>

/*const std::string WINDOW_TITLE = ENGINE_NAME + " " + ENGINE_VERSION;

void Croogine::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<Croogine*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}*/


namespace Croogine {

    CroogineWindow::CroogineWindow(int width, int height, std::string name) : width{ width }, height{ height }{  

        this->windowName = name;
        initWindow();
    }

    void CroogineWindow::initWindow() {
        //GLFW (Graphic Library FrameWork) is required to render the result within a window (online rendering).
        //glfwInit() will initialize the GLFW library.
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //By default, GLFW will generate an OpenGL context window. As we are using Vulkan instead of OpenGL
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr); //define window size and title (both are declared in Croogine_constants.h). No monitor for fulllscreen.
        
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    CroogineWindow::~CroogineWindow() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void CroogineWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }
    }


    bool CroogineWindow::shouldClose() { return glfwWindowShouldClose(window); }

    void CroogineWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
    {
        auto croogineWindow = reinterpret_cast<CroogineWindow*>(glfwGetWindowUserPointer(window));
        croogineWindow->framebufferResized = true;
        croogineWindow->width = width;
        croogineWindow->height = height;
    }
}