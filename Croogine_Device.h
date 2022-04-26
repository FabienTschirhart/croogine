#pragma once

#include "Croogine_window.h"

//std
#include <vector>
#include <vector>


namespace Croogine {

    struct QueueFamilyIndices {
        uint32_t graphicsFamily;
        uint32_t presentFamily;
        bool graphicsFamilyHasValue = false;
        bool presentFamilyHasValue = false;
        bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class CroogineDevice {

        public:

#ifdef NDEBUG
            const bool enableValidationLayers = false;
#else
            const bool enableValidationLayers = true;
#endif
            CroogineDevice(CroogineWindow& window);
            ~CroogineDevice();

            CroogineDevice(const CroogineDevice &) = delete;
            void operator=(const CroogineDevice &) = delete;
            CroogineDevice(CroogineDevice &&) = delete;
            CroogineDevice &operator=(CroogineDevice &&) = delete;

            VkCommandPool getCommandPool() { return commandPool; }
            VkDevice getDevice() { return device; }
            VkSurfaceKHR getSurface() { return surface; }
            VkQueue getGraphicsQueue() { return graphicsQueue; }
            VkQueue getPresentQueue() { return presentQueue; }

            SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice); }
            uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
            QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice); }

            VkFormat findSupportedFormat(
                const std::vector<VkFormat>& candidates, 
                VkImageTiling tiling, 
                VkFormatFeatureFlags features);

            void createBuffer(
                VkDeviceSize size,
                VkBufferUsageFlags usage,
                VkMemoryPropertyFlags properties,
                VkBuffer& buffer,
                VkDeviceMemory& bufferMemory);

            VkCommandBuffer beginSingleTimeCommands();
            void endSingleTimeCommands(VkCommandBuffer commandBuffer);

            void copyBuffer(
                VkBuffer srcBuffer, 
                VkBuffer dstBuffer, 
                VkDeviceSize size);

            void copyBufferToImage(
                VkBuffer buffer, 
                VkImage image, 
                uint32_t width, 
                uint32_t height, 
                uint32_t layerCount);

            void createImageWithInfo(
                const VkImageCreateInfo& imageInfo,
                VkMemoryPropertyFlags properties,
                VkImage& image,
                VkDeviceMemory& imageMemory);

            VkPhysicalDeviceProperties properties;

        private:
            void createInstance();
            void setupDebugMessenger();
            void createSurface();
            void pickPhysicalDevice();
            void createLogicalDevice();
            void createCommandPool();

            bool isDeviceSuitable(VkPhysicalDevice device);
            std::vector<const char*> getRequiredExtensions();
            bool checkValidationLayerSupport();
            QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
            void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
            void hasGflwRequiredInstanceExtensions();
            bool checkDeviceExtensionSupport(VkPhysicalDevice device);
            SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

            VkInstance instance;
            VkDebugUtilsMessengerEXT debugMessenger;
            VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
            CroogineWindow& window;
            VkCommandPool commandPool;

            VkDevice device;
            VkSurfaceKHR surface;
            VkQueue graphicsQueue;
            VkQueue presentQueue;

            const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
            const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    };
}