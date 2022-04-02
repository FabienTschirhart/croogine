#pragma once
#ifndef CROOGINE_BUFFERHANDLING
#define CROOGINE_BUFFERHANDLING

#include "Croogine_Core.h"

void createBuffer(VkDeviceSize, VkBufferUsageFlags, VkMemoryPropertyFlags, VkBuffer&, VkDeviceMemory&, VkDevice, VkPhysicalDevice);
void copyBuffer(VkBuffer, VkBuffer, VkDeviceSize, VkDevice, VkQueue, VkCommandPool);
VkCommandBuffer beginSingleTimeCommands(VkCommandPool commandPool, VkDevice device);
void endSingleTimeCommands(VkCommandBuffer commandBuffer, VkQueue graphicsQueue, VkDevice device, VkCommandPool commandPool);
void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, VkDevice device, VkQueue graphicsQueue, VkCommandPool commandPool);
#endif