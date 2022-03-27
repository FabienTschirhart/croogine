#pragma once
#ifndef CROOGINE_CONSTANTS
#define CROOGINE_CONSTANTS

#include "Croogine_Enums.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const int VERSION_VARIANT = 0;
const int VERSION_MAJOR = 0;
const int VERSION_MINOR = 1;
const int VERSION_PATCH = 0;

const std::string ENGINE_NAME = "Croogine 3D";
const std::string ENGINE_VERSION = "V0.0.1";

const uint32_t WIDTH = 1024;
const uint32_t HEIGHT = 768;

const GL_API SELECTED_GL_API = Vulkan;
const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};


#endif