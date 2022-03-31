#pragma once
#ifndef CROOGINE_BUFFERHANDLING
#define CROOGINE_BUFFERHANDLING

#include "Croogine_Core.h"

void createBuffer(VkDeviceSize, VkBufferUsageFlags, VkMemoryPropertyFlags, VkBuffer&, VkDeviceMemory&, VkDevice, VkPhysicalDevice);
void copyBuffer(VkBuffer, VkBuffer, VkDeviceSize, VkDevice, VkQueue, VkCommandPool);
#endif