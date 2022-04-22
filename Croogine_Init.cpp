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

    createRenderPipeline(vertexshader_file, fragmentshader_file); //per material

    createCommandPool();

    createDepthResources();
    createColorResources();
    createFramebuffers();

    createTextureImage(TEXTURE_PATH);
    createTextureImageView();
    createTextureSampler();

        loadModel(MODEL_PATH, vertices, indices);        // per model
        createVertexBuffer(); // per model
        createIndexBuffer(); // per model 

        createUniformBuffers(); // per model
        createDescriptorPool(); // per model 
        createDescriptorSets(); // per model 
        createCommandBuffers();// per model

    createSyncObjects(); // per model ??
}

void Croogine::recreateSwapChain()
{
    int width = 0, height = 0;
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(device);

    cleanupSwapChain();

    createSwapChain();
    createImageViews();
    createRenderPass();
    createRenderPipeline(vertexshader_file, fragmentshader_file);
    createColorResources();
    createDepthResources();
    createFramebuffers();
    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();
    createCommandBuffers();
}