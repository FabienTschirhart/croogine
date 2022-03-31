#include "Croogine_Core.h"
#include "Croogine_constants.h"

void Croogine::initGL()
{
    switch (SELECTED_GL_API)
    {
        case OpenGL:
            throw std::runtime_error("Selected Graphic Library (OpenGL) is not available");
            break;

        case Vulkan:
            initVulkan();
            break;

        case DirectX:
            throw std::runtime_error("Selected Graphic Library (DirectX) is not available");
            break;

        default :
            throw std::runtime_error("No Graphic Library selected");
            break;
    }
}

void Croogine::initOpenGL() {
};

void Croogine::initDirectX() {
};

void Croogine::initVulkan() {

    createInstance();

    setupDebugMessenger();

    createSurface();

    pickPhysicalDevice();

    createLogicalDevice();

    createSwapChain();

    createImageViews();

    createRenderPass();

    createDescriptorSetLayout();

    createRenderPipeline();

    createFramebuffers();

    createCommandPool();

    createVertexBuffer();

    createIndexBuffer();

    createUniformBuffers();

    createCommandBuffers();

    createSyncObjects();
}

void Croogine::recreateSwapChain()
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(device);
    
    cleanupSwapChain();

    createSwapChain();
    createImageViews();
    createRenderPass();
    createRenderPipeline();
    createFramebuffers();
}