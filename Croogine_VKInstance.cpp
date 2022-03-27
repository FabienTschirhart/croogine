#include "Croogine_Core.h"
#include "Croogine_constants.h"



void generateAppInfo(VkApplicationInfo &appinfo)
{
    appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appinfo.pApplicationName = "Croogine 3D Engine";
    appinfo.applicationVersion = VK_MAKE_API_VERSION(VERSION_VARIANT, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
    appinfo.pEngineName = "Croogine";
    appinfo.engineVersion = VK_MAKE_API_VERSION(VERSION_VARIANT, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
    appinfo.apiVersion = VK_API_VERSION_1_0;
}

void enumerateExtensions()
{
    uint32_t exCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &exCount, nullptr);
    std::vector<VkExtensionProperties> ex(exCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &exCount, ex.data());

    std::cout << "Available extensions :\n";

    for (const auto& extension : ex) {
        std::cout << '\t' << extension.extensionName << '\n';
    }

}

void Croogine::createInstance() {

    VkApplicationInfo appInfo{};
    VkInstanceCreateInfo instanceCInfo{};
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

    //validationlayer are used for debugging purposes only.
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    generateAppInfo(appInfo);

    enumerateExtensions();

    instanceCInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCInfo.pApplicationInfo = &appInfo;
    auto extensions = getRequiredExtensions();
    instanceCInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    instanceCInfo.ppEnabledExtensionNames = extensions.data();

    if (enableValidationLayers) {
        instanceCInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        instanceCInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        instanceCInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else {
        instanceCInfo.enabledLayerCount = 0;
        instanceCInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&instanceCInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

std::vector<const char*> Croogine::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    vkEnumerateInstanceExtensionProperties(nullptr, &glfwExtensionCount, nullptr);

    return extensions;
}
