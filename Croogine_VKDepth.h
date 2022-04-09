#pragma once


#ifndef CROOGINE_DEPTH
#define CROOGINE_DEPTH

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>




VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);

#endif